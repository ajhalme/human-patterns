#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include "opencv2/opencv.hpp"

#include "config.h"
#include "frameprocessor.h"

class HPPatternMatcher
{
public:
    HPPatternMatcher(HPConfig *config);
    ~HPPatternMatcher();

    void MatchSourceAndTarget(Mat source, Mat target, Mat *outFrames);
    void MaybeSaveBaseline(Mat source);

protected:
    HPConfig *config;
    cv::Mat baseline;
};

#endif // PATTERNMATCHER_H
