#ifndef HUMANPATTERNS_H
#define HUMANPATTERNS_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

#include "opencv2/opencv.hpp"
#include "common.h"
#include "config.h"
#include "frameprocessor.h"
#include "patternLoader.h"
#include "patternMatcher.h"
#include "soundPlayer.h"
#include "gameDisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HumanPatterns; }
QT_END_NAMESPACE

class HumanPatterns : public QMainWindow
{
    Q_OBJECT

public:
    // HumanPatterns(QWidget *parent = nullptr);
    HumanPatterns(QWidget *parent = nullptr, QString *configFile = nullptr);
    ~HumanPatterns();

    void LoadGame(QString gameDirectory);

protected:
    void closeEvent(QCloseEvent *event);

    void applyConfig();

    void openVideoByAddress();
    void processFrames();
    void processFrame(Mat *raw, Mat *source, Mat *target);    
    void displayScore(HPMatchScore score);

    void updateGame(HPMatchScore score);
    void completeGame();
    void advanceGame();

protected slots:
   void handleStart();

private slots:
   void on_showMarkersCheckBox_stateChanged(int);
   void on_showPlayAreaCheckBox_stateChanged(int);
   void on_captureButton_clicked();
   void on_clearButton_clicked();
   void on_launchGameDisplay_clicked();
   void on_saveBaseline_clicked();
   void on_saveConfig_clicked();
   void on_loadConfig_clicked();
   void on_blurSlider_valueChanged(int value);
   void on_threshSlider_valueChanged(int value);
   void on_startSoundButton_clicked();
   void on_levelSoundButton_clicked();
   void on_finishSoundButton_clicked();
   void on_audioToggle_stateChanged(int state);
   void on_patternSelection_clicked();
   void on_patternNext_clicked();
   void on_patternPrevious_clicked();
   void on_playPauseButton_clicked();
   void on_playResetButton_clicked();
   void on_playEndButton_clicked();
   void on_gameFree_clicked(bool checked);
   void on_gameSimple_clicked(bool checked);
   void on_gameChallenge_clicked(bool checked);
   void on_advanceSlider_valueChanged(int value);
   void on_minAreaSlider_valueChanged(int value);
   void on_rotationButton_clicked();

private:
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
    Ui::HumanPatterns *ui;

    QWidget *otherWindow;

    HPConfig *config;
    HPFrameProcessor *fp;
    HPPatternLoader *pl;
    HPPatternMatcher *pm;
    HPSoundPlayer *sp;

    HPGameDisplay *gameDisplay = nullptr;
};

#endif // HUMANPATTERNS_H
