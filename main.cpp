#include "humanpatterns.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    cout << "HUMAN PATTERNS - Antti Halme, 2019" << endl;

    QString configFile("/home/ajhalme/projects/humanpatterns-qt/config.yaml");

    vector<string> args(argv, argv+argc);
    if (args.size() == 2) {
        configFile = QString::fromStdString(args[1]);
    } else if (args.size() > 2) {
        cout << "Too many parameters. Terminating." << endl;
        return -1;
    }

    cout << "Loading configuration from: ";
    cout << configFile.toStdString();
    cout << endl;

    QApplication a(argc, argv);
    HumanPatterns w(nullptr, &configFile);
    w.show();
    return a.exec();
}
