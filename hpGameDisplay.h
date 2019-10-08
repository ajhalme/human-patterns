#ifndef HPGAMEDISPLAY_H
#define HPGAMEDISPLAY_H

#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <QLCDNumber>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "patternMatcher.h"

namespace Ui {
    class HPGameDisplay;
}

class HPGameDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit HPGameDisplay(QWidget *parent = nullptr);
    ~HPGameDisplay();

    void SetDisplay(HPMatchScore score, Mat *source, Mat* target, Mat* combined);
    void CenterToScreen();
    void PlayPause();
    void Reset();
    void Finish();

protected:
    Ui::HPGameDisplay *ui;
    int tickCounter;
    QGraphicsPixmapItem combinedPixmap;
    QGraphicsPixmapItem patternPixmap;
    QGraphicsPixmapItem inputPixmap;
    QTimer *timer = new QTimer(this);
    void setTimer(int ticks);
    void displayTicks();

private slots:
    void tick();
};

#endif // HPGAMEDISPLAY_H
