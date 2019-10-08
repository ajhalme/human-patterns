#include "common.h"
#include "gameDisplay.h"
#include "ui_hpgamedisplay.h"

HPGameDisplay::HPGameDisplay(QWidget *parent, HPConfig *config) :
    QWidget(parent),
    ui(new Ui::HPGameDisplay)
{
    this->config = config;
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
    Reset();

    prepareFinalScreen();

    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
}

void HPGameDisplay::PlayPause()
{
    if (timer->isActive())
        timer->stop();
    else
        timer->start(1000);
}

void HPGameDisplay::Finish()
{
    timer->stop();
    showFinalScreen();
}

void HPGameDisplay::showFinalScreen()
{
    combinedPixmap.setPixmap(hp::scaleToView(ui->combinedView, *finalScreen));
}

void HPGameDisplay::prepareFinalScreen()
{
    finalScreen = new QPixmap(config->gameEdgeSize, config->gameEdgeSize);
    QPainter painter(finalScreen);
    painter.fillRect(finalScreen->rect(), QColor(255, 255, 255));
    painter.setFont( QFont("Arial", 20) );
    painter.drawText(QPoint(25, 100), "Congratulations!" );
    painter.setFont( QFont("Arial", 12) );
    painter.drawText(QPoint(30, 150), "HUMAN PATTERNS 2019" );
}

void HPGameDisplay::resetView()
{
    QPixmap blank(config->gameEdgeSize, config->gameEdgeSize);
    QPainter painter(&blank);
    painter.fillRect(blank.rect(), QColor(255, 255, 255));
    combinedPixmap.setPixmap(hp::scaleToView(ui->combinedView, blank));
}

void HPGameDisplay::Reset()
{
    timer->stop();
    setTimer(5*60);
    resetView();
}

void HPGameDisplay::setTimer(int ticks)
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

void HPGameDisplay::SetDisplay(HPMatchScore score, Mat *source, Mat* target, Mat* combined)
{
    if (!timer->isActive()) return;

    ui->scorePositive->display(score.score_true_pos);
    ui->scoreNegative->display(score.score_false_pos);
    ui->scoreQuality->display(score.quality);

    QPixmap pmap = hp::frame2Img(combined);
    combinedPixmap.setPixmap(hp::scaleToView(ui->combinedView, pmap));

    pmap = hp::frame2Img(source);
    inputPixmap.setPixmap(hp::scaleToView(ui->inputView, pmap));

    pmap = hp::frame2Img(target);
    patternPixmap.setPixmap(hp::scaleToView(ui->patternView, pmap));
}

void HPGameDisplay::CenterToScreen() {
  QList<QScreen *> screens = QGuiApplication::screens();
  if (screens.length() < 2) return;

  QScreen *current, *external;

  current = QGuiApplication::screenAt(QCursor::pos());

  if (current->name() != screens[0]->name())
      external = screens[0];
  else
      external = screens[1];

  QRect geom = external->availableGeometry();

  int desk_x = geom.width();
  int desk_y = geom.height();
  int x = this->width();
  int y = this->height();
  this->move(desk_x/2 - x/2 + geom.left(), desk_y/2 - y/2 + geom.top());
}

