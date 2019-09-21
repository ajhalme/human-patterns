#include "common.h"
#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
{    
    ui->setupUi(this);
    this->setGeometry(100,100,1600,800);

    ui->captureGroup->setEnabled(false);
    ui->gameGroup->setEnabled(false);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);

    ui->scoreNeg->setDigitCount(2);
    ui->scorePos->setDigitCount(2);
    ui->scoreQuality->setDigitCount(2);

    startSound = new QMediaPlayer();
    startSound->setMedia(QUrl("qrc:/sounds/sounds/start.wav"));
    levelSound = new QMediaPlayer();
    levelSound->setMedia(QUrl("qrc:/sounds/sounds/level.wav"));
    finishSound = new QMediaPlayer();
    finishSound->setMedia(QUrl("qrc:/sounds/sounds/complete.wav"));

    config = new HPConfig(HPConfig::SmallSize, HPConfig::PatternSize);
    fp = new HPFrameProcessor(config);
    pl = new HPPatternLoader(config);
    pm = new HPPatternMatcher(config);

    connect(ui->startButton, SIGNAL (released()), this, SLOT (handleStart()));

    on_loadConfig_clicked();

    pm->LoadBaselineFile();
    LoadPattern(config->debugPatternFile);
}

void HumanPatterns::centerToScreen(QWidget* widget) {
  if (!widget)
    return;
  QList<QScreen *> screens = QGuiApplication::screens();
  if (screens.length() < 2) return;

  QScreen *external;
  QScreen *current = QGuiApplication::screenAt(QCursor::pos());
  if (current->name() != screens[0]->name())
      external = screens[0];
  else
      external = screens[1];

  QRect geom = external->availableGeometry();

  int desk_x = geom.width();
  int desk_y = geom.height();
  int x = widget->width();
  int y = widget->height();
  widget->move(desk_x/2 - x/2 + geom.left(), desk_y/2 - y/2 + geom.top());
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
    int cameraIndex = ui->videoSourceEdit->text().toInt(&isCamera);
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
    return ui->videoSourceEdit->text().trimmed().toStdString();
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

            QPixmap img = hp::frame2Img(&frame);
            pixmap.setPixmap(img);
        }
        qApp->processEvents();
    }
}

void HumanPatterns::processFrame(Mat *raw, Mat *source, Mat *outFrames)
{
    if (!config->playAreaReady) {
        fp->ProcessRaw(raw, outFrames);
        return;
    }

    fp->ProcessPlayArea(raw, source);
    pm->MaybeSaveBaselineFile(source);

    HPMatchScore score = pm->MatchSourceAndTarget(source, pl->Current(), outFrames);
    displayScore(score);
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
    config->capturePlayArea = true;
    ui->captureButton->setEnabled(false);
}
void HumanPatterns::on_clearButton_clicked()
{
    config->capturePlayArea = false;
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
    gameDisplay->showMaximized();
    centerToScreen(gameDisplay);
    config->gameWindowOpen = true;
    ui->gameGroup->setEnabled(true);
}
void HumanPatterns::on_saveBaseline_clicked()
{
    config->saveBaseline = true;
}

void HumanPatterns::devDebug() { // DEBUG0
    qApp->processEvents();

    pl->LoadPatternFile(QFileInfo("../humanpatterns-qt/patterns/hp-pattern-1.svg.png"));

    qApp->processEvents();

    Mat frame;
    Mat raw = imread(config->debugFile.toStdString());

    qApp->processEvents();

    HPMatchScore score = pm->MatchSourceAndTarget(&raw, pl->Current(), &frame);
    displayScore(score);

    QPixmap img = hp::frame2Img(&frame);
    pixmap.setPixmap(img);
}

void HumanPatterns::displayScore(HPMatchScore score)
{
    ui->scorePos->display(score.score_true_pos);
    ui->scoreNeg->display(score.score_false_pos);
    ui->scoreQuality->display(score.quality);

    if (config->gameWindowOpen)
        gameDisplay->SetDisplay(score, &pm->thresh, pl->Current(), &pm->combined);
}

void HumanPatterns::on_blurSlider_valueChanged(int blurValue)
{
    QString qs;
    config->blurValue = blurValue;
    ui->blurLabel->setText(qs.sprintf("Blur: %02d", blurValue));    
}
void HumanPatterns::on_threshSlider_valueChanged(int threshValue)
{
    QString qs;
    config->threshValue = threshValue;
    ui->threshLabel->setText(qs.sprintf("Thresh: %02d", threshValue));
}

void HumanPatterns::on_saveConfig_clicked()
{
    FileStorage fs(config->persistenceFile, FileStorage::WRITE);
    fs.open(config->persistenceFile, FileStorage::WRITE);

    fs << "blurValue" << config->blurValue;
    fs << "threshValue" << config->threshValue;

    fs.release();
}

void HumanPatterns::on_loadConfig_clicked()
{
    FileStorage fs(config->persistenceFile, FileStorage::READ);
    fs.open(config->persistenceFile, FileStorage::READ);

    fs["blurValue"] >> config->blurValue;
    on_blurSlider_valueChanged(config->blurValue);
    ui->blurSlider->setValue(config->blurValue);

    fs["threshValue"] >> config->threshValue;
    on_threshSlider_valueChanged(config->threshValue);
    ui->threshSlider->setValue(config->threshValue);

    fs.release();
}

void HumanPatterns::on_resetTimerButton_clicked()
{
    gameDisplay->ResetTimer();
}

void HumanPatterns::on_stopButton_clicked()
{
    gameDisplay->StopTimer();
}

void HumanPatterns::playSound(QMediaPlayer *player)
{
    if (!config->audioOn) return;

    if (player->state() == QMediaPlayer::PlayingState)
        player->stop();
    player->setPosition(0);
    player->play();
}

void HumanPatterns::on_startSoundButton_clicked()
{
    playSound(startSound);
}

void HumanPatterns::on_levelSoundButton_clicked()
{
    playSound(levelSound);
}

void HumanPatterns::on_finishSoundButton_clicked()
{
    playSound(finishSound);
}

void HumanPatterns::on_audioToggle_stateChanged(int value)
{
    config->audioOn = value > 0;
}
