#ifndef HPGAMEDISPLAY_H
#define HPGAMEDISPLAY_H

#include <QScreen>
#include <QTimer>
#include <QWidget>
#include <QLCDNumber>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#include "config.h"
#include "patternMatcher.h"

namespace Ui {
    class HPGameDisplay;
}

class HPGameDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit HPGameDisplay(QWidget *parent = nullptr, HPConfig *config = nullptr);
    ~HPGameDisplay();

    void SetDisplay(HPMatchScore score, Mat *source, Mat* target, Mat* combined);
    void CenterToScreen();
    void PlayPause();
    void Reset();
    void Finish();

protected:
    HPConfig *config;
    Ui::HPGameDisplay *ui;
    int tickCounter;
    QGraphicsPixmapItem combinedPixmap;
    QGraphicsPixmapItem patternPixmap;
    QGraphicsPixmapItem inputPixmap;
    QTimer *timer = new QTimer(this);
    void setTimer(int ticks);
    void displayTicks();

    QPixmap *finalScreen;
    void prepareFinalScreen();
    void showFinalScreen();

    void resetView();

private slots:
    void tick();
};

#endif // HPGAMEDISPLAY_H
