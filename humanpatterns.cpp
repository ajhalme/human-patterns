#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
{
    ui->setupUi(this);

    ui->captureGroup->setEnabled(false);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
    // IgnoreAspectRatio  KeepAspectRatio  KeepAspectRatioByExpanding

    config = new HPConfig(HPConfig::SmallSize, HPConfig::PatternSize);
    fp = new HPFrameProcessor(config);
    pl = new HPPatternLoader(config);
    pm = new HPPatternMatcher(config);

    connect(ui->startButton, SIGNAL (released()), this, SLOT (handleStart()));

    devDebug();
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

QPixmap frame2Img(Mat *frame)
{
    QImage qimg(frame->data, frame->cols, frame->rows, static_cast<int>(frame->step),
                QImage::Format_RGB888);
    return QPixmap::fromImage(qimg.rgbSwapped());
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

void HumanPatterns::processFrames()
{
    using namespace cv;

    Mat raw, frame;
    Mat source = Mat(config->targetSize, HPConfig::HPImageType);

    while(video.isOpened())
    {
        video >> raw;
        if(!raw.empty())
        {            
            qApp->processEvents();

            frame.release();
            processFrame(&raw, &source, &frame);

            qApp->processEvents();

            QPixmap img = frame2Img(&frame);
            pixmap.setPixmap(img);
        }
        qApp->processEvents();
    }
}

void HumanPatterns::processFrame(Mat *raw, Mat *source, Mat *target)
{
    if (!config->playAreaReady) {
        fp->ProcessRaw(raw, target);
        return;
    }

    fp->ProcessPlayArea(raw, source);
    pm->MaybeSaveBaselineFile(source);
    pm->MatchSourceAndTarget(source, pl->Current(), target);
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

void HumanPatterns::LoadPattern(QString patternFileName)
{
    QFileInfo patternFile(patternFileName);
    ui->patternLabel->setText(patternFile.completeBaseName());
    pl->LoadPatternFile(patternFile);
}

void HumanPatterns::on_patternButton_clicked()
{
    // TODO: load directory

    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Select Pattern File",
                config->patternDirectory,
                "HP Pattern (*.png);;All Files (*)");

    LoadPattern(fileName);
}

void HumanPatterns::on_launchGameDisplay_clicked()
{
    gameDisplay = new HPGameDisplay(this);
    gameDisplay->setWindowFlags(Qt::Window);
    gameDisplay->show();

}
void HumanPatterns::on_saveBaseline_clicked()
{
    config->saveBaseline = true;
}



void HumanPatterns::devDebug() { // DEBUG
    pm->LoadBaselineFile();
    pl->LoadPatternFile(QFileInfo("../humanpatterns-qt/patterns/hp-pattern-1.svg.png"));

    Mat frame;
    Mat raw = imread(config->debugFile.toStdString());

    pm->MatchSourceAndTarget(&raw, pl->Current(), &frame);

    QPixmap img = frame2Img(&frame);
    pixmap.setPixmap(img);
}
