#include "patternMatcher.h"

using namespace cv;
using namespace std;

HPPatternMatcher::HPPatternMatcher(HPConfig *config)
{
    this->config = config;
    targetPattern = Mat(config->targetSize, CV_8UC3);
}

void HPPatternMatcher::LoadPatternFile(QFileInfo patternFile)
{
    auto pathString = patternFile.absoluteFilePath().toStdString();
    Mat rawPattern = cv::imread(String(pathString), cv::IMREAD_COLOR);
    TransformPattern(&rawPattern);
}

void HPPatternMatcher::TransformPattern(Mat *rawPattern)
{
    Mat M = getPerspectiveTransform(config->patternShape, config->targetShape);
    warpPerspective(*rawPattern, targetPattern, M, config->targetSize);
}

void HPPatternMatcher::getTarget(Mat *output)
{
    targetPattern.copyTo(*output);
}
