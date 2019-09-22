#ifndef CONFIG_H
#define CONFIG_H

#include "opencv2/opencv.hpp"
#include <QString>
#include "common.h"

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
    bool gameWindowOpen = false;

    bool saveBaseline = false;
    const QString baselineFile = "../humanpatterns-qt/baseline.png";
    const QString debugFile = "../humanpatterns-qt/debug.png";
    const QString patternDirectory = "../humanpatterns-qt/patterns";
    const QString gamesDirectory = "../humanpatterns-qt/games";
    const QString debugPatternFile = "../humanpatterns-qt/patterns/hp-pattern-1.svg.png";
    const QString debugGameDirectory = "../humanpatterns-qt/games/debug";

    const string persistenceFile = "../humanpatterns-qt/config.yaml";

    int blurValue = 5;
    int threshValue = 20;
    bool audioOn = true;
    HPGameMode gameMode = HPGameMode::Free;
    int advanceThreshold = 70;
    int minPatternScore = 1500;

    bool gameComplete = true;
    bool gamePaused = true;
};

#endif // CONFIG_H
