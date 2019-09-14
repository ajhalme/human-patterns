#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QFrame>

namespace Ui {
class GameDisplay;
}

class HPGameDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit HPGameDisplay(QWidget *parent = nullptr);
    ~HPGameDisplay();

private:
    Ui::GameDisplay *ui;
};

#endif // GAMEDISPLAY_H
