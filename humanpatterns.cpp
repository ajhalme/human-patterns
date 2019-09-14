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

    config = new HPConfig(HPConfig::SmallSize, HPConfig::PatternSize);
    fp = new HPFrameProcessor(config);
    pl = new HPPatternLoader(config);
    pm = new HPPatternMatcher(config);

    connect(ui->startButton, SIGNAL (released()), this, SLOT (handleStart()));

    ui->captureGroup->setEnabled(false);
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
            "Stop video capture before closing the application!");
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

    ui->startButton->setEnabled(false);

    qApp->processEvents();

    if (video.isOpened()) {
        video.release();
        pixmap.hide();
        ui->startButton->setText("Start");
        ui->captureGroup->setEnabled(false);
    } else {
        openVideoByAddress();
        pixmap.show();
        ui->startButton->setText("Stop");
        ui->captureGroup->setEnabled(true);
    }

    ui->startButton->setEnabled(true);


    qApp->processEvents();

    processFrames();
}

std::string HumanPatterns::GetAddress()
{
    return ui->videoEdit->text().trimmed().toStdString();
}

std::string HumanPatterns::GetState()
{
    return ui->startButton->text().trimmed().toStdString();
}

QPixmap frame2Img(cv::Mat frame)
{
    QImage qimg(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step),
                QImage::Format_RGB888);
    return QPixmap::fromImage(qimg.rgbSwapped());
}

void HumanPatterns::processFrames()
{
    using namespace cv;

    Mat raw, frame;
    Mat source = Mat(config->targetSize, CV_8UC3);
    Mat target = Mat(config->targetSize, CV_8UC3);

    while(video.isOpened())
    {
        video >> raw;
        if(!raw.empty())
        {            
            qApp->processEvents();

            if (config->playAreaReady) {
                fp->ProcessPlayArea(raw, &source);                
                pm->MaybeSaveBaseline(source);
                frame.release();
                pm->MatchSourceAndTarget(source, pl->Current(), &frame);
            }
            else
                fp->ProcessRaw(raw, &frame);

            qApp->processEvents();

            QPixmap img = frame2Img(frame);
            pixmap.setPixmap(img);
        }
        qApp->processEvents();
    }
}

void HumanPatterns::on_showMarkersCheckBox_stateChanged(int)
{
    config->showDetectedMarkers = !config->showDetectedMarkers;
}
void HumanPatterns::on_showPlayAreaCheckBox_stateChanged(int)
{
    config->showDetectedPlayArea = !config->showDetectedPlayArea;
}
void HumanPatterns::on_captureButton_clicked()
{
    config->capturePlayArea = !config->capturePlayArea;
    ui->captureButton->setEnabled(false);
}
void HumanPatterns::on_clearButton_clicked()
{
    config->playAreaReady = false;
    ui->captureButton->setEnabled(true);
}

void HumanPatterns::on_patternButton_clicked()
{
    // TODO: load directory

    QString fileName = QFileDialog::getOpenFileName(this,
        "Select Pattern File", "../humanpatterns-qt/patterns", "HP Pattern (*.png);;All Files (*)");
    QFileInfo patternFile(fileName);

    ui->patternLabel->setText(patternFile.completeBaseName());

    pl->LoadPatternFile(patternFile);
}

void HumanPatterns::on_launchGameDisplay_clicked()
{
    gameDisplay = new HPGameDisplay(this);
    gameDisplay->setWindowFlags(Qt::Window);
    gameDisplay->show();

}
