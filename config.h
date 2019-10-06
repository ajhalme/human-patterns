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
    const static int RotationOrientations = 4;

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
    const QString baselineFile = "/home/ajhalme/projects/humanpatterns-qt/baseline.png";
    const QString debugFile = "/home/ajhalme/projects/humanpatterns-qt/debug.png";
    const QString patternDirectory = "/home/ajhalme/projects/humanpatterns-qt/patterns";
    const QString gamesDirectory = "/home/ajhalme/projects/humanpatterns-qt/games";
    const QString debugPatternFile = "/home/ajhalme/projects/humanpatterns-qt/patterns/hp-pattern-1.svg.png";
    const QString debugGameDirectory = "/home/ajhalme/projects/humanpatterns-qt/games/debug";

    const string persistenceFile = "/home/ajhalme/projects/humanpatterns-qt/config.yaml";

    int blurValue = 5;
    int threshValue = 20;
    bool audioOn = true;
    HPGameMode gameMode = HPGameMode::Free;
    int advanceThreshold = 70;
    int minPatternScore = 1500;
    int rotation = 0;

    bool gameComplete = true;
    bool gamePaused = true;
};

#endif // CONFIG_H
