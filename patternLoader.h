#ifndef PATTERNLOADER_H
#define PATTERNLOADER_H

#include <QFileInfo>
#include <QDir>
#include "opencv2/opencv.hpp"

#include "config.h"

class HPPatternLoader
{
public:
    HPPatternLoader(HPConfig *config);
    ~HPPatternLoader();

    Mat *Current();

    void LoadPatternFile(QFileInfo patternFile);
    void LoadGameDirectory(QDir gameDirectory);
    void Next();
    void Previous();

protected:
    cv::Mat targetPattern;
    void transformPattern(Mat *rawPattern);

    HPConfig *config;
    QDir currentGame;
    uint patternIndex;
    uint totalPatterns;
    vector<cv::Mat> gamePatterns;
};


#endif // PATTERNLOADER_H
