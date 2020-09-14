#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QIcon>

#include <vector>
#include <algorithm>

#include "farmblock.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    int timerate;
    int globaltime;
    int globaltimer;
    int infmon;
    int infrate;
    int totpignum;
    int money;
    int Req_addpig;
    int gamemode;
    float prices[3];
    farmblock farms[100];
    bool isPause;

    int page;
    int blockid[10];
    int detailid;
    QString recordname;

    std::vector<farmblock*> heap;
    std::vector<farmblock*> bheap;

    QLabel* labeltime;
    QLabel* labelmoney;
    QLabel* farmdetail;
    QPushButton* PauseBut;
    QPushButton* blockshow[100];
    QPushButton* StartBut;
    QPushButton* SpeedBut;
    QPushButton* add_pig;
    QPushButton* pagenext;
    QPushButton* pagepre;
    QPushButton* statisticbut;

    void timerEvent(QTimerEvent *);

    void balanced_insert(pig*&);
    void addpig();
    pig* buypig(int&,int);

    void showfarmdetail(int);
    void mainmenu();
    void buildblocks();
    void changepage(bool);
    void viewcreate();

    void debugout();
    QPushButton* testbut;
    QLabel* testlabel;

    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void gamestart();
    void reqaddpig();
    void changeTimeRate();
    void savefile();
    void pac();
    void statistic();

    void slottest();
};

#endif // MAINWINDOW_H
