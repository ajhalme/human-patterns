#include "common.h"
#include "hpGameDisplay.h"
#include "ui_hpgamedisplay.h"

HPGameDisplay::HPGameDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPGameDisplay)
{
    ui->setupUi(this);

    ui->combinedView->setScene(new QGraphicsScene(this));
    ui->combinedView->scene()->addItem(&combinedPixmap);

    ui->inputView->setScene(new QGraphicsScene(this));
    ui->inputView->scene()->addItem(&inputPixmap);

    ui->patternView->setScene(new QGraphicsScene(this));
    ui->patternView->scene()->addItem(&patternPixmap);

    ui->scorePositive->setDigitCount(2);
    ui->scoreNegative->setDigitCount(2);
    ui->scoreQuality->setDigitCount(2);

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

QPixmap scaleToView(QGraphicsView *view, QPixmap pmap)
{
    QSize sz = view->size();
    return pmap.scaled(sz.width() - 1, sz.height() - 4, Qt::KeepAspectRatio);
}

void HPGameDisplay::SetDisplay(HPMatchScore score, Mat *source, Mat* target, Mat* combined)
{
    ui->scorePositive->display(score.score_true_pos);
    ui->scoreNegative->display(score.score_false_pos);
    ui->scoreQuality->display(score.quality);

    QPixmap pmap = hp::frame2Img(combined);
    combinedPixmap.setPixmap(scaleToView(ui->combinedView, pmap));

    pmap = hp::frame2Img(source);
    inputPixmap.setPixmap(scaleToView(ui->inputView, pmap));

    pmap = hp::frame2Img(target);
    patternPixmap.setPixmap(scaleToView(ui->patternView, pmap));
}
