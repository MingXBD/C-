#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.setFixedSize(1280,768);
    w.setWindowTitle("PigFarm");
    w.show();
    return a.exec();
}
