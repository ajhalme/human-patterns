#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include "opencv2/opencv.hpp"

#include "common.h"
#include "config.h"
#include "frameprocessor.h"

class HPMatchScore
{
private:
    Mat scoring;
    int PixelScore(Mat* frame);

public:
    HPMatchScore(Mat* base, Mat* pos, Mat* neg, HPConfig *config);

    int score_pattern;
    int score_positive_diff;
    int score_negative_diff;

    int score_true_pos;
    int score_false_pos;
    int quality;
    int spill;
};

class HPPatternMatcher
{
public:
    HPPatternMatcher(HPConfig *config);
    ~HPPatternMatcher();

    Mat thresh, combined;

    HPMatchScore MatchSourceAndTarget(Mat *source, Mat *target, Mat *outFrames);
    void MaybeSaveBaselineFile(Mat *source);
    void SaveBaselineFile(Mat *source);
    void LoadBaselineFile();

protected:
    Mat Blank();

    HPConfig *config;
    Mat baseline;

    Mat basedelta, fusion, tfusion, targeti;
    Mat matchDiff, matchDiffInv, matchDiff2, matchDiff2Inv;
};

#endif // PATTERNMATCHER_H
