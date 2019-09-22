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
    gamePatterns.clear();

    QStringList gps = gameDirectory.entryList();
    Mat M = getPerspectiveTransform(config->patternShape, config->targetShape);
    Mat patternTemplate = Mat(config->targetSize, HPConfig::HPImageType);

    for_each(gps.begin(), gps.end(),
        [&] ( QString gamePattern ) {
            QString gpPath = gameDirectory.filePath(gamePattern);
            QFileInfo gpFile(gpPath);
            if (!gpFile.isFile()) return;

            Mat rawPattern = cv::imread(gpPath.toStdString(), cv::IMREAD_COLOR);
            warpPerspective(rawPattern, patternTemplate, M, config->targetSize);
            gamePatterns.push_back(patternTemplate.clone());
        }
    );

    patternIndex = 0;
    totalPatterns = uint(gamePatterns.size());
    hasPatterns = totalPatterns > 0;

    if (hasPatterns)
        targetPattern = gamePatterns.at(patternIndex);
}

void HPPatternLoader::Next()
{
    if (!hasPatterns) return;

    if (patternIndex < totalPatterns - 1)
        patternIndex++;
    targetPattern = gamePatterns.at(patternIndex);
}

void HPPatternLoader::Previous()
{
    if (!hasPatterns) return;

    if (patternIndex > 0)
        patternIndex--;
    targetPattern = gamePatterns.at(patternIndex);
}

void HPPatternLoader::SetPatternState(QLabel *obj)
{
    if (!hasPatterns)
        obj->setText("N/A");
    else
    {
        QString idx = QString::number(patternIndex + 1);
        QString total = QString::number(totalPatterns);
        obj->setText(QString("Pat %1/%2").arg(idx, total));
    }
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
