#include "frameprocessor.h"

using namespace cv;
using namespace std;

HPFrameProcessor::HPFrameProcessor()
{
    dict = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);
    playAreaCorners = {};
    markerIds = {};
}

bool sortClockWise(Point2f a, Point2f b, Point mid)
{
    float aang = atan2(a.y - mid.y, a.x - mid.x);
    float bang = atan2(b.y - mid.y, b.x - mid.x);
    return aang < bang;
}

vector<Point2f> getPlayArea(Mat frame, vector<vector<Point2f>> markers)
{
    vector<Point2f> corners;

    Point mid = Point(frame.size[1]/2, frame.size[0]/2);

    for (vector<Point2f> &marker : markers)
    {
        double lowestDistance = numeric_limits<double>::max();
        Point2f chosen;
        for (Point2f &corner : marker)
        {
            float ax = mid.x - corner.x;
            float ay = mid.y - corner.y;
            double distance = sqrt(pow(ax, 2) + pow(ay, 2));
            if (distance < lowestDistance)
            {
                chosen = corner;
                lowestDistance = distance;
            }
        }
        corners.push_back(chosen);
    }
    sort(corners.begin(), corners.end(),
         [&mid](Point2f x, Point2f y) -> bool { return sortClockWise(x, y, mid); });

    return corners;
}

void drawPlayArea(Mat frame, vector<vector<Point2f>> corners)
{
    vector<Point2f> playarea = getPlayArea(frame, corners);
    for (size_t i = 0; i < 4; i++) {
        Point2f a = playarea[i];
        Point2f b = playarea[(i+1) % 4];
        line(frame, a, b, Scalar(0, 255, 0));
    }
}

void applySquareTransform(Mat input, Mat *output, vector<vector<Point2f>> points, HPConfig *config)
{
    vector<Point2f> playarea = getPlayArea(input, points);
    Mat M = getPerspectiveTransform(playarea, config->targetShape);
    warpPerspective(input, *output, M, config->targetSize);
}

void HPFrameProcessor::CacheState(cv::Mat frame, vector<int> ids,
        vector<vector<Point2f>> corners, HPConfig *config)
{
    markerIds = ids;
    playAreaCorners = corners;
    baseline = Mat(config->targetShape, CV_64FC1);
    applySquareTransform(frame, &baseline, corners, config);
}

Mat HPFrameProcessor::ProcessRaw(Mat frame, HPConfig *config)
{
    vector<int> ids;
    vector<vector<Point2f>> corners;

    cv::aruco::detectMarkers(frame, dict, corners, ids);

    if (config->showDetectedMarkers)
        cv::aruco::drawDetectedMarkers(frame, corners, ids);

    bool isQuatric = ids.size() == 4;

    if (!isQuatric) return frame;

    if (config->showDetectedPlayArea)
        drawPlayArea(frame, corners);

    if (config->capturePlayArea) {
        CacheState(frame, ids, corners, config);
        config->capturePlayArea = false;
        config->playAreaReady = true;
    }

    return frame;
}

Mat HPFrameProcessor::ProcessPlayArea(Mat frame, HPConfig *config)
{
    Mat output = Mat(config->targetShape, CV_64FC1);
    applySquareTransform(frame, &output, playAreaCorners, config);
    return output;
}
