#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std; 

class HPConfig
{
public:
    const static int SmallSize = 250;
    const static int PatternSize = 750;

public:
    HPConfig(int edge, int patternEdge);
    ~HPConfig();

    Size targetSize;
    vector<Point2f> targetShape;
    Size patternSize;
    vector<Point2f> patternShape;

    bool showDetectedMarkers = false;
    bool showDetectedPlayArea = false;

    bool capturePlayArea = false;
    bool playAreaReady = false;
};

#endif // CONFIG_H
