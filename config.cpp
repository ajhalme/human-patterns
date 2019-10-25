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
    fs << "hackValue" << hackValue;

    fs << "baseDir " << baseDir.toStdString();
    fs << "patternDirectory " << patternDirectory.toStdString();
    fs << "gamesDirectory " << gamesDirectory.toStdString();
    fs << "defaultGamesDirectory " << defaultGamesDirectory.toStdString();
    fs << "cameraAddress " << cameraAddress;

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
    fs["hackValue"] >> hackValue;

    string readStr;
    fs["baseDir"] >> readStr;
    baseDir = QString::fromStdString(readStr);
    baselineFile = baseDir + "baseline.png";
    fs["patternDirectory"] >> readStr;
    patternDirectory = QString::fromStdString(readStr);
    fs["gamesDirectory"] >> readStr;
    gamesDirectory = QString::fromStdString(readStr);
    fs["defaultGamesDirectory"] >> readStr;
    defaultGamesDirectory = QString::fromStdString(readStr);
    fs["cameraAddress"] >> cameraAddress;

    fs.release();
}

void HPConfig::Print()
{
    cout << "blurValue " << blurValue << endl;
    cout << "threshValue " << threshValue << endl;
    cout << "gameMode " << gameMode << endl;
    cout << "advanceThreshold " << advanceThreshold << endl;
    cout << "audioOn " << audioOn << endl;
    cout << "minPatternScore " << minPatternScore << endl;
    cout << "rotation " << rotation << endl;
    cout << "hackValue " << hackValue << endl;

    cout << "baseDir " << baseDir.toStdString() << endl;
    cout << "patternDirectory " << patternDirectory.toStdString() << endl;
    cout << "gamesDirectory " << gamesDirectory.toStdString() << endl;
    cout << "defaultGamesDirectory " << defaultGamesDirectory.toStdString() << endl;
    cout << "cameraAddress " << cameraAddress << endl;
}

HPConfig::~HPConfig()
{
}
