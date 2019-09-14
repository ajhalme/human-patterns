#include "gamedisplay.h"
#include "ui_gamedisplay.h"

HPGameDisplay::HPGameDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GameDisplay)
{
    ui->setupUi(this);
}

HPGameDisplay::~HPGameDisplay()
{
    delete ui;
}
