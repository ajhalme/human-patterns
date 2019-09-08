#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);

    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
    // IgnoreAspectRatio  KeepAspectRatio  KeepAspectRatioByExpanding

    fp = new HPFrameProcessor();

    connect(ui->startButton, SIGNAL (released()), this, SLOT (handleStart()));
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

void HumanPatterns::openVideoByCameraIndex()
{
    bool isCamera;
    int cameraIndex = ui->videoEdit->text().toInt(&isCamera);
    if(isCamera)
    {
        if(!video.open(cameraIndex))
        {
            QMessageBox::critical(this,
                "Camera Error",
                "Make sure you entered a correct camera index,"
                "<br>or that the camera is not being accessed by another program!");
            return;
        }
    }
}

void HumanPatterns::openVideoByAddress()
{
    std::string address = GetAddress();
    if(!video.open(address))
    {
        QMessageBox::critical(this,
            "Video Error",
            "Make sure you entered a correct and supported video file path,"
            "<br>or a correct RTSP feed URL!");
        return;
    }
}

void HumanPatterns::handleStart()
{
    // rtsp://192.168.0.111:554/11
    // rtsp://192.168.0.111:554/12

    toggleButtonString();

    ui->startButton->setEnabled(false);

    qApp->processEvents();

    if (video.isOpened()) {
        video.release();
    } else {
        openVideoByAddress();
    }

    ui->startButton->setEnabled(true);

    qApp->processEvents();

    processFrames();
}

void HumanPatterns::toggleButtonString()
{
    const std::string start = "Start";
    const std::string stop = "Stop";

    std::string address = GetState();

    if (address.compare(start) == 0)
        ui->startButton->setText(stop.c_str());
    else
        ui->startButton->setText(start.c_str());
}

std::string HumanPatterns::GetAddress()
{
    return ui->videoEdit->text().trimmed().toStdString();
}

std::string HumanPatterns::GetState()
{
    return ui->startButton->text().trimmed().toStdString();
}

void HumanPatterns::processFrames()
{
    using namespace cv;

    Mat raw;
    while(video.isOpened())
    {
        video >> raw;
        if(!raw.empty())
        {
            Mat frame = fp->ProcessFrame(raw);

            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            QPixmap img = QPixmap::fromImage(qimg.rgbSwapped());
            pixmap.setPixmap(img);
        }
        qApp->processEvents();
    }
}

