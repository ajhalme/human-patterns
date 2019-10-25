#include "common.h"
#include "humanpatterns.h"
#include "ui_humanpatterns.h"

HumanPatterns::HumanPatterns(QWidget *parent, QString *configFile)
    : QMainWindow(parent)
    , ui(new Ui::HumanPatterns)
    , config(new HPConfig(configFile))
{       
    ui->setupUi(this);
    this->setGeometry(100,100,1600,800);

    ui->captureGroup->setEnabled(false);
    // ui->gameGroup->setEnabled(false);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);

    ui->scoreNeg->setDigitCount(2);
    ui->scorePos->setDigitCount(2);
    ui->scoreQuality->setDigitCount(2);

    fp = new HPFrameProcessor(config);
    pl = new HPPatternLoader(config);
    pm = new HPPatternMatcher(config);
    sp = new HPSoundPlayer(config);

    connect(ui->startButton, SIGNAL (released()), this, SLOT (handleStart()));

    applyConfig();
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

void HumanPatterns::openVideoByAddress()
{
    if(!video.open(config->cameraAddress))
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
            pixmap.setPixmap(hp::scaleToView(ui->graphicsView, img));
        }
        qApp->processEvents();
    }
}

void static StarkWhiteToBlack(Mat *mat, HPConfig *config)
{
    Mat mask = Mat(mat->size(), HPConfig::HPImageType, double(0));

    Scalar col = Scalar(200, 50, 50);

    double brightBound = config->hackValue;
    Scalar bright = Scalar(brightBound, brightBound, brightBound);

    double notBrightBound = 255; //config->hackValue;
    Scalar notBright = Scalar(notBrightBound, notBrightBound, notBrightBound);

    cv::inRange(*mat, bright, notBright, mask);
    mat->setTo(col, mask);
}

void HumanPatterns::processFrame(Mat *raw, Mat *source, Mat *outFrames)
{
    if (!config->playAreaReady) {
        fp->ProcessRaw(raw, outFrames);
        return;
    }

    fp->ProcessPlayArea(raw, source);
    pm->MaybeSaveBaselineFile(source);

    StarkWhiteToBlack(source, config);

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
    sp->PlayFinish();
    config->gameComplete = true;
    config->gamePaused = true;

    if (gameDisplay != nullptr)
        gameDisplay->Finish();

    ui->currentPatternLabel->setText("Done!");
    ui->playPauseButton->setText("PLAY");    
}

void HumanPatterns::advanceGame()
{
    if (config->gameMode == HPGameMode::Free) return;

    sp->PlayLevelUp();
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

void HumanPatterns::LoadGame(QString gameDirectoryPath)
{
    QDir gameDirectory(gameDirectoryPath);
    ui->gameDirectoryEdit->setText(gameDirectoryPath);
    pl->LoadGameDirectory(gameDirectory);
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());
}

void HumanPatterns::on_launchGameDisplay_clicked()
{   
    gameDisplay = new HPGameDisplay(this, config);
    gameDisplay->setWindowFlags(Qt::Window);
    gameDisplay->showMaximized();
    gameDisplay->CenterToScreen();
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
    config->Save();
}

void HumanPatterns::applyConfig()
{
    on_blurSlider_valueChanged(config->blurValue);
    ui->blurSlider->setValue(config->blurValue);

    on_threshSlider_valueChanged(config->threshValue);
    ui->threshSlider->setValue(config->threshValue);

    on_hackSlider_valueChanged(config->hackValue);
    ui->hackSlider->setValue(config->hackValue);

    switch (config->gameMode) {
        case HPGameMode::Free: ui->gameFree->setChecked(true); break;
        case HPGameMode::Simple: ui->gameSimple->setChecked(true); break;
        case HPGameMode::Challenge: ui->gameChallenge->setChecked(true); break;
    }

    ui->audioToggle->setChecked(config->audioOn);

    on_advanceSlider_valueChanged(config->advanceThreshold);
    ui->advanceSlider->setValue(config->advanceThreshold);

    on_minAreaSlider_valueChanged(config->minPatternScore);
    ui->minAreaSlider->setValue(config->minPatternScore);

    config->rotation--; // adjust for click
    on_rotationButton_clicked();

    if (config->defaultGamesDirectory.length() > 0)
        LoadGame(config->defaultGamesDirectory);

    if (config->cameraAddress.length() > 0)
        ui->videoSourceEdit->setText(QString::fromStdString(config->cameraAddress));
}

void HumanPatterns::on_loadConfig_clicked()
{
    config->Load();
    applyConfig();
}

void HumanPatterns::on_startSoundButton_clicked()
{
    sp->PlayStart();
}
void HumanPatterns::on_levelSoundButton_clicked()
{
    sp->PlayLevelUp();
}
void HumanPatterns::on_finishSoundButton_clicked()
{
    sp->PlayFinish();
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

    if (gameDirectory.length() > 1)
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
        sp->PlayStart();
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
    ui->playPauseButton->setText("PLAY");

    pl->Reset();
    ui->currentPatternLabel->setText(pl->GetPatternStateSummary());

    if (gameDisplay == nullptr) return;
    gameDisplay->Reset();

}

void HumanPatterns::on_playEndButton_clicked()
{
    config->gameComplete = true;
    config->gamePaused = true;

    completeGame();

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

void HumanPatterns::on_hackSlider_valueChanged(int value)
{
    QString qs;
    config->hackValue = value;
    ui->hackLabel->setText(qs.sprintf("Hack %04d", value));
}
