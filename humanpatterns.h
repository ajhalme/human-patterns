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

#include "opencv2/opencv.hpp"

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

private:
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
    Ui::HumanPatterns *ui;
};
#endif // HUMANPATTERNS_H
