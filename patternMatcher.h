#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include <QFileInfo>
#include "opencv2/opencv.hpp"

#include "config.h"
#include "frameprocessor.h"

class HPPatternMatcher
{
public:
    HPPatternMatcher(HPConfig *config);
    ~HPPatternMatcher();

    void LoadPatternFile(QFileInfo patternFile);

    void getTarget(Mat *out);

protected:
    HPConfig *config;
    cv::Mat targetPattern;

    void TransformPattern(Mat *rawPattern);
};

#endif // PATTERNMATCHER_H
