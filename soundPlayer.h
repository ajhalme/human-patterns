#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "config.h"
#include <QMediaPlayer>

class HPSoundPlayer
{
public:
    HPSoundPlayer(HPConfig *config);
    ~HPSoundPlayer();

    void PlayStart();
    void PlayLevelUp();
    void PlayFinish();

protected:
    HPConfig *config;

    QMediaPlayer *startSound;
    QMediaPlayer *levelSound;
    QMediaPlayer *finishSound;

    void playSound(QMediaPlayer *player);
};


#endif // SOUNDPLAYER_H
