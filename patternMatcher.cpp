#include "patternMatcher.h"

using namespace cv;
using namespace std;

HPPatternMatcher::HPPatternMatcher(HPConfig *config)
{
    this->config = config;
}

void HPPatternMatcher::MatchSourceAndTarget(Mat source, Mat target, Mat *outFrames)
{
    Mat arr[] = {baseline, source, target};
    hconcat(arr, 3, *outFrames);
}

void HPPatternMatcher::MaybeSaveBaseline(Mat source)
{
    if (!config->capturePlayArea) return;

    source.copyTo(baseline);

    config->capturePlayArea = false;
}

