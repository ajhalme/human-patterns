#ifndef HUMANPATTERNS_H
#define HUMANPATTERNS_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>

#include "opencv2/opencv.hpp"
#include "config.h"
#include "frameprocessor.h"
#include "patternLoader.h"
#include "patternMatcher.h"
#include "hpGameDisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HumanPatterns; }
QT_END_NAMESPACE

class HumanPatterns : public QMainWindow
{
    Q_OBJECT

public:
    HumanPatterns(QWidget *parent = nullptr);
    ~HumanPatterns();

    void LoadPattern(QString patternFileName);

protected:
    void closeEvent(QCloseEvent *event);

    void devDebug();
    void openVideoByCameraIndex();
    void openVideoByAddress();
    void toggleButtonString();
    std::string GetAddress();
    std::string GetState();

    void processFrames();
    void processFrame(Mat *raw, Mat *source, Mat *target);
    void displayScore(HPMatchScore score);

protected slots:
   void handleStart();

private slots:
   void on_showMarkersCheckBox_stateChanged(int);
   void on_showPlayAreaCheckBox_stateChanged(int);
   void on_captureButton_clicked();
   void on_clearButton_clicked();

   void on_patternButton_clicked();

   void on_launchGameDisplay_clicked();

   void on_saveBaseline_clicked();

private:
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
    Ui::HumanPatterns *ui;

    QWidget *otherWindow;

    HPConfig *config;
    HPFrameProcessor *fp;
    HPPatternLoader *pl;
    HPPatternMatcher *pm;
    HPGameDisplay *gameDisplay;
};

#endif // HUMANPATTERNS_H
