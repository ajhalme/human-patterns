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
    LoadGame(config->debugGameDirectory);
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

    updateGame(score);
}

void HumanPatterns::updateGame(HPMatchScore score)
{
    if (score.quality < config->advanceThreshold) return;
    if (config->gameComplete) return;

    if (pl->OnLastPattern())
        completeGame();
    else
        advanceGame();
}

void HumanPatterns::completeGame()
{
    playSound(finishSound);
    config->gameComplete = true;
    config->gamePaused = true;
    gameDisplay->Finish();
    ui->currentPatternLabel->setText("Done!");
    ui->playPauseButton->setText("PLAY");

    // TODO: Results display
}

void HumanPatterns::advanceGame()
{
    playSound(levelSound);
    pl->Next();
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());
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

void HumanPatterns::LoadGame(QString gameDirectoryPath)
{
    QDir gameDirectory(gameDirectoryPath);
    ui->gameDirectoryEdit->setText(gameDirectoryPath);
    pl->LoadGameDirectory(gameDirectory);
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());
}

void HumanPatterns::on_patternButton_clicked()
{
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
    fs << "gameMode" << config->gameMode;
    fs << "advanceThreshold" << config->advanceThreshold;
    fs << "audioOn" << config->audioOn;
    fs << "minPatternScore" << config->minPatternScore;
    fs << "rotation" << config->rotation;

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

    fs["gameMode"] >> config->gameMode;
    switch (config->gameMode) {
        case HPGameMode::Free: ui->gameFree->setChecked(true); break;
        case HPGameMode::Simple: ui->gameSimple->setChecked(true); break;
        case HPGameMode::Challenge: ui->gameChallenge->setChecked(true); break;
    }

    fs["audioOn"] >> config->audioOn;
    ui->audioToggle->setChecked(config->audioOn);

    fs["advanceThreshold"] >> config->advanceThreshold;
    on_advanceSlider_valueChanged(config->advanceThreshold);
    ui->advanceSlider->setValue(config->advanceThreshold);

    fs["minPatternScore"] >> config->minPatternScore;
    on_minAreaSlider_valueChanged(config->minPatternScore);
    ui->minAreaSlider->setValue(config->minPatternScore);

    fs["rotation"] >> config->rotation;
    config->rotation--; // adjust for click
    on_rotationButton_clicked();

    fs.release();
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

void HumanPatterns::on_audioToggle_stateChanged(int state)
{
    config->audioOn = state > 0;
}

void HumanPatterns::on_patternSelection_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    QString gameDirectory = dialog.getExistingDirectory(
                this,
                "Select Game Directory",
                config->gamesDirectory);

    LoadGame(gameDirectory);
}

void HumanPatterns::on_patternNext_clicked()
{
    pl->Next();
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());
}

void HumanPatterns::on_patternPrevious_clicked()
{
    pl->Previous();
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());
}

void HumanPatterns::on_playPauseButton_clicked()
{
    if (config->gamePaused) {
        playSound(this->startSound);
        config->gameComplete = false;
        config->gamePaused = false;
        ui->playPauseButton->setText("PAUSE");
    } else {
        config->gamePaused = true;
        ui->playPauseButton->setText("PLAY");
    }

    if (gameDisplay == nullptr) return;
    gameDisplay->PlayPause();
}

void HumanPatterns::on_playResetButton_clicked()
{
    config->gameComplete = false;
    config->gamePaused = true;

    pl->Reset();
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());

    if (gameDisplay == nullptr) return;
    gameDisplay->Reset();

}

void HumanPatterns::on_playEndButton_clicked()
{
    config->gameComplete = true;
    config->gamePaused = true;

    if (gameDisplay == nullptr) return;
    gameDisplay->Finish();
}

void HumanPatterns::on_gameFree_clicked(bool checked)
{
    if (!checked) return;
    config->gameMode = HPGameMode::Free;
}

void HumanPatterns::on_gameSimple_clicked(bool checked)
{
    if (!checked) return;
    config->gameMode = HPGameMode::Simple;
}

void HumanPatterns::on_gameChallenge_clicked(bool checked)
{
    if (!checked) return;
    config->gameMode = HPGameMode::Challenge;
}

void HumanPatterns::on_advanceSlider_valueChanged(int advanceThreshold)
{
    QString qs;
    config->advanceThreshold = advanceThreshold;
    ui->advanceLabel->setText(qs.sprintf("Adv.Q : %02d", advanceThreshold));
}

void HumanPatterns::on_minAreaSlider_valueChanged(int minArea)
{
    QString qs;
    config->minPatternScore = minArea;
    ui->minAreaLabel->setText(qs.sprintf("MinArea %04d", minArea));
}

void HumanPatterns::on_rotationButton_clicked()
{
    QString qs;
    config->rotation = (1 + config->rotation) % config->RotationOrientations;
    ui->rotationButton->setText(qs.sprintf("Rot %d", config->rotation));
}

