#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class HPConfig
{
public:
    const static int SmallSize = 250;

public:
    HPConfig(int edge);
    ~HPConfig();

    Size targetSize;
    vector<Point2f> targetShape;

    bool showDetectedMarkers = false;
    bool showDetectedPlayArea = false;

    bool capturePlayArea = false;
    bool playAreaReady = false;
};

#endif // CONFIG_H
