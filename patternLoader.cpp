#include "patternLoader.h"

using namespace cv;
using namespace std;


HPPatternLoader::HPPatternLoader(HPConfig *config)
{
    this->config = config;
    targetPattern = Mat(config->targetSize, HPConfig::HPImageType);
    gamePatterns = {};
}

void HPPatternLoader::LoadPatternFile(QFileInfo patternFile)
{
    auto pathString = patternFile.absoluteFilePath().toStdString();
    Mat rawPattern = cv::imread(String(pathString), cv::IMREAD_COLOR);
    transformPattern(&rawPattern);
}

void HPPatternLoader::LoadGameDirectory(QDir gameDirectory)
{
    currentGame = gameDirectory;
    totalPatterns = currentGame.count() - 2 - 1;
    gamePatterns.clear();
    auto gps = gameDirectory.entryList();
    Mat M = getPerspectiveTransform(config->patternShape, config->targetShape);

    for_each(gps.begin(), gps.end(),
        [&] ( QString gamePattern ) {
            QString gpPath = gameDirectory.filePath(gamePattern);
            QFileInfo gpFile(gpPath);
            if (!gpFile.isFile()) return;

            Mat patternTemplate = Mat(config->targetSize, HPConfig::HPImageType);
            Mat rawPattern = cv::imread(gpPath.toStdString(), cv::IMREAD_COLOR);
            warpPerspective(rawPattern, patternTemplate, M, config->targetSize);
            gamePatterns.push_back(patternTemplate.clone());
        }
    );

    patternIndex = 0;
    if (totalPatterns > 0)
        targetPattern = gamePatterns.at(patternIndex);
}

void HPPatternLoader::Next()
{
    if (patternIndex < totalPatterns)
        patternIndex++;
    targetPattern = gamePatterns.at(patternIndex);
}

void HPPatternLoader::Previous()
{
    if (patternIndex > 0)
        patternIndex--;
    targetPattern = gamePatterns.at(patternIndex);
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
