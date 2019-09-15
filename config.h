#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/opencv.hpp"
#include <QString>

using namespace cv;
using namespace std; 

class HPConfig
{
public:
    const static int SmallSize = 250;
    const static int PatternSize = 750;
    const static int HPImageType = CV_8UC3;

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

    bool saveBaseline = false;
    const QString baselineFile = "../humanpatterns-qt/baseline.png";
    const QString debugFile = "../humanpatterns-qt/debug.png";
    const QString patternDirectory = "../humanpatterns-qt/patterns";

    const string persistenceFile = "../humanpatterns-qt/config.yaml";

    int blurValue = 5;
};

#endif // CONFIG_H
