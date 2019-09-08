#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
{
    ui->setupUi(this);
}

HumanPatterns::~HumanPatterns()
{
    delete ui;
}

