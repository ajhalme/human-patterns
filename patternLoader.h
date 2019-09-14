#ifndef PATTERNLOADER_H
#define PATTERNLOADER_H

#include <QFileInfo>
#include "opencv2/opencv.hpp"

#include "config.h"

class HPPatternLoader
{
public:
    HPPatternLoader(HPConfig *config);
    ~HPPatternLoader();

    void LoadPatternFile(QFileInfo patternFile);
    Mat Current();

protected:
    HPConfig *config;
    cv::Mat targetPattern;

    void transformPattern(Mat *rawPattern);
};


#endif // PATTERNLOADER_H
