#ifndef HPGAMEDISPLAY_H
#define HPGAMEDISPLAY_H

#include <QTimer>
#include <QWidget>
#include <QLCDNumber>

namespace Ui {
    class HPGameDisplay;
}

class HPGameDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit HPGameDisplay(QWidget *parent = nullptr);
    ~HPGameDisplay();

private:
    Ui::HPGameDisplay *ui;
    int tickCounter;

private slots:
    void tick();
    void resetTimer(int ticks);
    void displayTicks();
};

#endif // HPGAMEDISPLAY_H
