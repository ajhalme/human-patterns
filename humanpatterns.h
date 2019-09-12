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
#include "patternMatcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HumanPatterns; }
QT_END_NAMESPACE

class HumanPatterns : public QMainWindow
{
    Q_OBJECT

public:
    HumanPatterns(QWidget *parent = nullptr);
    ~HumanPatterns();

protected:
    void closeEvent(QCloseEvent *event);
    void openVideoByCameraIndex();
    void openVideoByAddress();
    void toggleButtonString();
    std::string GetAddress();
    std::string GetState();

    void processFrames();

protected slots:
   void handleStart();

private slots:
   void on_showMarkersCheckBox_stateChanged(int);
   void on_showPlayAreaCheckBox_stateChanged(int);
   void on_captureButton_clicked();
   void on_clearButton_clicked();

   void on_patternButton_clicked();

private:
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
    Ui::HumanPatterns *ui;
    HPFrameProcessor *fp;
    HPConfig *config;
    HPPatternMatcher *pm;
};

#endif // HUMANPATTERNS_H
