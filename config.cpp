#include "config.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

vector<Point2f> shapeFromSize(Size s)
{
    return {
        Point2f(0,              0),
        Point2f(s.width,        0),
        Point2f(s.width, s.height),
        Point2f(0,       s.height)
    };
}

HPConfig::HPConfig(QString *hpConfigFile)
{
    configFile = *hpConfigFile;
    Load();
    setSizesShapes(gameEdgeSize, patternEdgeSize);
    Print();
}

void HPConfig::setSizesShapes(int edge, int patternEdge)
{
    const Size s = Size(edge, edge);
    targetSize = s;
    targetShape = shapeFromSize(s);

    const Size ps = Size(patternEdge, patternEdge);
    patternSize = ps;
    patternShape = shapeFromSize(ps);
}

void HPConfig::Save()
{
    string hpFileStr(configFile.toStdString());
    FileStorage fs(hpFileStr, FileStorage::WRITE);
    fs.open(hpFileStr, FileStorage::WRITE);

    fs << "blurValue" << blurValue;
    fs << "threshValue" << threshValue;
    fs << "gameMode" << gameMode;
    fs << "advanceThreshold" << advanceThreshold;
    fs << "audioOn" << audioOn;
    fs << "minPatternScore" << minPatternScore;
    fs << "rotation" << rotation;

    fs.release();
}

void HPConfig::Load()
{
    string hpFileStr(configFile.toStdString());
    FileStorage fs(hpFileStr, FileStorage::READ);
    fs.open(hpFileStr, FileStorage::READ);

    fs["blurValue"] >> blurValue;
    fs["threshValue"] >> threshValue;
    fs["gameMode"] >> gameMode;
    fs["audioOn"] >> audioOn;
    fs["advanceThreshold"] >> advanceThreshold;
    fs["minPatternScore"] >> minPatternScore;
    fs["rotation"] >> rotation;

    string baseDirStr, patternDirectoryStr, gamesDirectoryStr;
    fs["baseDir"] >> baseDirStr;
    baseDir = QString::fromStdString(baseDirStr);
    baselineFile = baseDir + "baseline.png";
    fs["patternDirectory"] >> patternDirectoryStr;
    patternDirectory = QString::fromStdString(patternDirectoryStr);
    fs["gamesDirectory"] >> gamesDirectoryStr;
    gamesDirectory = QString::fromStdString(gamesDirectoryStr);

    fs.release();
}

void HPConfig::Print()
{
    cout << "blurValue" << blurValue;
    cout << "threshValue" << threshValue;
    cout << "gameMode" << gameMode;
    cout << "advanceThreshold" << advanceThreshold;
    cout << "audioOn" << audioOn;
    cout << "minPatternScore" << minPatternScore;
    cout << "rotation" << rotation;
}

HPConfig::~HPConfig()
{
}
