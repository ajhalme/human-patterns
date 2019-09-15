#ifndef HPGAMEDISPLAY_H
#define HPGAMEDISPLAY_H

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

protected:
    Ui::HPGameDisplay *ui;
    int tickCounter;
    QGraphicsPixmapItem combinedPixmap;
    QGraphicsPixmapItem patternPixmap;
    QGraphicsPixmapItem inputPixmap;

private slots:
    void tick();
    void resetTimer(int ticks);
    void displayTicks();
};

#endif // HPGAMEDISPLAY_H
