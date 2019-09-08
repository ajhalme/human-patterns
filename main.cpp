#include "humanpatterns.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HumanPatterns w;
    w.show();
    return a.exec();
}
