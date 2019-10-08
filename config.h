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
    const static int HPImageType = CV_8UC3;
    const static int RotationOrientations = 4;

public:
    HPConfig(QString *hpConfigFile);
    ~HPConfig();

    void Save();
    void Load();
    void Print();

    Size targetSize;
    vector<Point2f> targetShape;
    Size patternSize;
    vector<Point2f> patternShape;

    bool showDetectedMarkers = false;
    bool showDetectedPlayArea = false;

    bool capturePlayArea = false;
    bool gameWindowOpen = false;
    bool playAreaReady = false;
    bool saveBaseline = false;
    bool gameComplete = true;
    bool gamePaused = true;

    QString baseDir = "";
    QString baselineFile = "";
    QString patternDirectory = "";
    QString gamesDirectory = "";
    QString defaultGamesDirectory = "";

    string cameraAddress = "";

    int gameEdgeSize = 250;
    int patternEdgeSize = 750;
    int blurValue = 5;
    int threshValue = 20;
    bool audioOn = true;
    HPGameMode gameMode = HPGameMode::Free;
    int advanceThreshold = 70;
    int minPatternScore = 1500;
    int rotation = 0;

private:
    QString configFile;
    void setSizesShapes(int edge, int patternEdge);
};

#endif // CONFIG_H
