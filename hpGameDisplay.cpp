#include "hpGameDisplay.h"
#include "ui_hpgamedisplay.h"

HPGameDisplay::HPGameDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPGameDisplay)
{
    ui->setupUi(this);

    ui->lcdTimer->setDigitCount(5);
    resetTimer(5*60);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void HPGameDisplay::resetTimer(int ticks)
{
    tickCounter = ticks;
    displayTicks();
}

void HPGameDisplay::tick()
{
    if (tickCounter <= 0) return;

    tickCounter--;
    displayTicks();
}

void HPGameDisplay::displayTicks()
{
    int minutes = tickCounter / 60;
    int seconds = tickCounter % 60;
    QString qs;
    qs.sprintf("%02d:%02d", minutes, seconds);
    ui->lcdTimer->display(qs);
}

HPGameDisplay::~HPGameDisplay()
{
    delete ui;
}
