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

    void MatchSourceAndTarget(Mat *source, Mat *target, Mat *outFrames);
    void MaybeSaveBaselineFile(Mat *source);
    void SaveBaselineFile(Mat *source);
    void LoadBaselineFile();

protected:
    Mat Blank();

    HPConfig *config;
    Mat baseline;

    Mat basedelta, thresh;
    Mat fusion, tfusion;
    Mat matchDiff, matchDiffInv, matchDiff2, matchDiff2Inv;
};

#endif // PATTERNMATCHER_H