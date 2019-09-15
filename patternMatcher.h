#ifndef PATTERNMATCHER_H
#define PATTERNMATCHER_H

#include "opencv2/opencv.hpp"

#include "config.h"
#include "frameprocessor.h"

class HPMatchScore
{
private:
    Mat scoring;
    int PixelScore(Mat* frame);

public:
    HPMatchScore(Mat* base, Mat* pos, Mat* neg);

    int score_pattern;
    int score_positive_diff;
    int score_negative_diff;

    double score_true_pos;
    double score_false_pos;
    double quality;
    double spill;
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
