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
}

Mat HPPatternMatcher::Blank()
{
    return Mat(config->targetSize, HPConfig::HPImageType);
}

void HPPatternMatcher::MaybeSaveBaselineFile(Mat *source)
{
    if (!config->saveBaseline) return;
    SaveBaselineFile(source);
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

void HPPatternMatcher::MatchSourceAndTarget(Mat *source, Mat *target, Mat *outFrames)
{    
    cv::subtract(baseline, *source, basedelta);
    cv::cvtColor(basedelta, basedelta, COLOR_BGR2GRAY);
    cv::blur(basedelta, basedelta, Size(5,5));
    cv::threshold(basedelta, thresh, 20, 255, THRESH_BINARY_INV);
    cv::cvtColor(basedelta, basedelta, COLOR_GRAY2BGR);
    cv::cvtColor(thresh, thresh, COLOR_GRAY2BGR);
    cv::bitwise_not(basedelta, basedelta);

    cv::addWeighted(*source, 0.75, *target, 0.25, 0, fusion);
    cv::addWeighted(thresh, 0.75, *target, 0.75, 0, tfusion);

    cv::subtract(thresh, *target, matchDiff);
    cv::subtract(*target, thresh, matchDiff2);
    cv::bitwise_not(matchDiff, matchDiffInv);
    cv::bitwise_not(matchDiff2, matchDiff2Inv);

    Mat upper, lower;
    vector<Mat> upperRow = {baseline, *source, *target, basedelta, thresh};
    vector<Mat> lowerRow = {fusion, tfusion, matchDiff, matchDiffInv, matchDiff2Inv};

    hconcat(upperRow, upper);
    hconcat(lowerRow, lower);
    vector<Mat> rows = {upper, lower};
    vconcat(rows, *outFrames);
}
