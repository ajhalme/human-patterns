#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
}

HumanPatterns::~HumanPatterns()
{
    delete ui;
}

void HumanPatterns::closeEvent(QCloseEvent *event)
{
    if(video.isOpened())
    {
        QMessageBox::warning(this,
            "Warning",
            "Stop the video before closing the application!");
            event->ignore();
    }
    else
    {
        event->accept();
    }
}
