#include "patternMatcher.h"

using namespace cv;
using namespace std;

HPPatternMatcher::HPPatternMatcher(HPConfig *config)
{
    this->config = config;
    baseline = Blank();
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
    vector<Mat> mats = {baseline, *source, *target};

    Mat delta = Blank();
    cv::subtract(baseline, *source, delta);

    mats.push_back(delta);

    hconcat(mats, *outFrames);
}
