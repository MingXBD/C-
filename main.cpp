#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    w.setFixedSize(1280,768);
    w.setWindowTitle("PigFarm");
    w.show();
    return a.exec();
}
