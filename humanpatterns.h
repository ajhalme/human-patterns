#ifndef HUMANPATTERNS_H
#define HUMANPATTERNS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HumanPatterns; }
QT_END_NAMESPACE

class HumanPatterns : public QMainWindow
{
    Q_OBJECT

public:
    HumanPatterns(QWidget *parent = nullptr);
    ~HumanPatterns();

private:
    Ui::HumanPatterns *ui;
};
#endif // HUMANPATTERNS_H
