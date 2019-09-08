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

vector<Point2f> getPlayArea(Point mid, vector<vector<Point2f>> markers)
{
    vector<Point2f> corners;
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

Mat HPFrameProcessor::ProcessFrame(Mat frame)
{
    Mat markered;
    vector<int> ids;
    vector<vector<Point2f>> corners, rejected;
    Ptr<aruco::DetectorParameters> parameters;

    frame.copyTo(markered);
    cv::aruco::detectMarkers(frame, dict, corners, ids);

    if (ids.size() > 0)
        cv::aruco::drawDetectedMarkers(markered, corners, ids);

    if (ids.size() == 4)
    {
        Point mid = Point(frame.size[1]/2, frame.size[0]/2);
        vector<Point2f> playarea = getPlayArea(mid, corners);
        for (size_t i = 0; i < 4; i++) {
            Point2f a = playarea[i];
            Point2f b = playarea[(i+1) % 4];
            line(markered, a, b, Scalar(0, 255, 0));
        }
    }

    return markered;
}
