#include "patternLoader.h"

using namespace cv;
using namespace std;

HPPatternLoader::HPPatternLoader(HPConfig *config)
{
    this->config = config;
    targetPattern = Mat(config->targetSize, HPConfig::HPImageType);
}

void HPPatternLoader::LoadPatternFile(QFileInfo patternFile)
{
    auto pathString = patternFile.absoluteFilePath().toStdString();
    Mat rawPattern = cv::imread(String(pathString), cv::IMREAD_COLOR);
    transformPattern(&rawPattern);
}

void HPPatternLoader::transformPattern(Mat *rawPattern)
{
    Mat M = getPerspectiveTransform(config->patternShape, config->targetShape);
    warpPerspective(*rawPattern, targetPattern, M, config->targetSize);
}

Mat *HPPatternLoader::Current()
{
   return &targetPattern;
}
