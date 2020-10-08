#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.setFixedSize(1280,768);
    w.setWindowIcon(QIcon(":/res/icon.png"));
    w.setWindowTitle("开局一条狗 (虽然并没有）");
    w.show();
    return a.exec();
}
