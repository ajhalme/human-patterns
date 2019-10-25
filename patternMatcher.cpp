#include "patternMatcher.h"

using namespace cv;
using namespace std;

HPPatternMatcher::HPPatternMatcher(HPConfig *config)
{
    this->config = config;
    baseline = Blank();

    basedelta = Blank();
    thresh = Blank();
    fusion = Blank();
    tfusion = Blank();
    matchDiff = Blank();
    matchDiffInv = Blank();
    matchDiff2 = Blank();
    matchDiff2Inv = Blank();
    combined = Blank();
}

Mat HPPatternMatcher::Blank()
{
    return Mat(config->targetSize, HPConfig::HPImageType);
}

void HPPatternMatcher::MaybeSaveBaselineFile(Mat *source)
{
    if (!config->saveBaseline) return;
    SaveBaselineFile(source);
    LoadBaselineFile();
    config->saveBaseline = false;
}
void HPPatternMatcher::SaveBaselineFile(Mat *source)
{
    imwrite(config->baselineFile.toStdString(), *source);
}
void HPPatternMatcher::LoadBaselineFile()
{
    baseline = imread(config->baselineFile.toStdString());
}

void static TintFrames(Mat *target, Mat *targeti, Mat *matchDiff, Mat* matchDiff2, Mat* combined)
{
    Mat mask;
    Scalar blue = Scalar(222,184,143);
    Scalar red = Scalar(145,145,222);
    Scalar purple = Scalar(148,80,155);
    Scalar tint_lower = Scalar(128, 128, 128);
    Scalar tint_upper = Scalar(255, 255, 255);
    target->copyTo(*combined);
    cv::inRange(*targeti, tint_lower, tint_upper, mask);
    combined->setTo(purple, mask);
    cv::inRange(*matchDiff, tint_lower, tint_upper, mask);
    matchDiff->setTo(blue, mask);
    combined->setTo(blue, mask);
    cv::inRange(*matchDiff2, tint_lower, tint_upper, mask);
    matchDiff2->setTo(red, mask);
    combined->setTo(red, mask);
}

HPMatchScore HPPatternMatcher::MatchSourceAndTarget(Mat *source, Mat *target, Mat *outFrames)
{
    cv::subtract(baseline, *source, basedelta);    

    cv::cvtColor(basedelta, basedelta, COLOR_BGR2GRAY);
    cv::blur(basedelta, basedelta, Size(config->blurValue, config->blurValue));    

    cv::threshold(basedelta, thresh, config->threshValue, 255, THRESH_BINARY_INV);
    cv::cvtColor(basedelta, basedelta, COLOR_GRAY2BGR);

    cv::cvtColor(thresh, thresh, COLOR_GRAY2BGR);
    cv::bitwise_not(basedelta, basedelta);

    // Blend
    cv::addWeighted(*source, 0.75, *target, 0.25, 0, fusion);
    cv::addWeighted(thresh, 0.75, *target, 0.75, 0, tfusion);

    // Diffs
    cv::subtract(thresh, *target, matchDiff);
    cv::subtract(*target, thresh, matchDiff2);
    cv::bitwise_not(*target, targeti);
    cv::bitwise_not(matchDiff, matchDiffInv);
    cv::bitwise_not(matchDiff2, matchDiff2Inv);

    // Tint
    TintFrames(target, &targeti, &matchDiff, &matchDiff2, &combined);

    // Prepare for drawing
    Mat upper, lower;
    vector<Mat> upperRow = {baseline, *source, *target, basedelta, thresh};
    vector<Mat> lowerRow = {fusion, targeti, matchDiff, matchDiff2, combined};
    hconcat(upperRow, upper);
    hconcat(lowerRow, lower);
    vector<Mat> rows = {upper, lower};
    vconcat(rows, *outFrames);

    return HPMatchScore(&targeti, &matchDiff, &matchDiff2, config);
}

HPMatchScore::HPMatchScore(Mat* base, Mat* pos, Mat* neg, HPConfig *config)
{
    score_pattern = PixelScore(base);
    score_positive_diff = PixelScore(pos);
    score_negative_diff = PixelScore(neg);

    score_true_pos = static_cast<int>(100 - round((100.0 * score_positive_diff) / score_pattern));

    int minScorePattern = config->minPatternScore * 10;
    score_pattern = score_pattern < minScorePattern ? minScorePattern : score_pattern;

    score_false_pos = static_cast<int>(round((100.0 * score_negative_diff) / score_pattern));

    switch (config->gameMode) {
        case HPGameMode::Challenge:
            quality = int(round(score_true_pos - (0.5)*score_false_pos));
            break;
        case HPGameMode::Simple:
        case HPGameMode::Free:
            quality = int(round(score_true_pos));
            break;
    }

    if (quality < 0) quality = 0;
    if (quality > 99) quality = 99;
    if (score_false_pos > 99) score_false_pos = 99;
    if (score_true_pos > 99) score_true_pos = 99;

    spill = score_false_pos;
}

int HPMatchScore::PixelScore(Mat* frame)
{
    frame->copyTo(scoring);
    cv::cvtColor(scoring, scoring, COLOR_BGR2GRAY);
    return cv::countNonZero(scoring);
}
