#include "soundPlayer.h"

HPSoundPlayer::HPSoundPlayer(HPConfig *config)
{
    this->config = config;

    startSound = new QMediaPlayer();
    startSound->setMedia(QUrl("qrc:/sounds/sounds/start.wav"));
    levelSound = new QMediaPlayer();
    levelSound->setMedia(QUrl("qrc:/sounds/sounds/level.wav"));
    finishSound = new QMediaPlayer();
    finishSound->setMedia(QUrl("qrc:/sounds/sounds/complete.wav"));
}

void HPSoundPlayer::PlayStart()
{
    playSound(startSound);
}
void HPSoundPlayer::PlayLevelUp()
{
    playSound(levelSound);
}
void HPSoundPlayer::PlayFinish()
{
    playSound(finishSound);
}

void HPSoundPlayer::playSound(QMediaPlayer *player)
{
    if (!config->audioOn) return;

    if (player->state() == QMediaPlayer::PlayingState)
        player->stop();
    player->setPosition(0);
    player->play();
}
