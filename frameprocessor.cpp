#include "frameprocessor.h"

using namespace cv;
using namespace std;

HPFrameProcessor::HPFrameProcessor()
{
    dict = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);
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

void applyTransform(Mat input, Mat *output, vector<vector<Point2f>> points, HPConfig config)
{
    vector<Point2f> playarea = getPlayArea(input, points);
    Mat M = getPerspectiveTransform(playarea, config.targetShape);
    warpPerspective(input, *output, M, config.targetSize);
}

Mat HPFrameProcessor::ProcessFrame(Mat frame, HPConfig config)
{
    Mat processed;
    vector<int> ids;
    vector<vector<Point2f>> corners;

    frame.copyTo(processed);

    cv::aruco::detectMarkers(frame, dict, corners, ids);

    bool isQuatric = ids.size() == 4;

    if (config.showDetectedMarkers && ids.size() > 0)
        cv::aruco::drawDetectedMarkers(processed, corners, ids);

    if (config.showDetectedPlayArea && isQuatric)
        drawPlayArea(processed, corners);

    if (config.applyTransforms && isQuatric)
    {
        Mat output = Mat(config.targetShape, CV_64FC1);
        applyTransform(frame, &output, corners, config);
        return output;
    }

    return processed;
}
