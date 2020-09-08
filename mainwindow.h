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

    void timerEvent(QTimerEvent *);


    void balanced_insert(pig*&);
    void addpig();
    pig* buypig(int&,int);

    void showfarmdetail(int);
    void mainmenu();
    void buildblocks();
    void statistic();
    void showData();

    int debugswitch;
    void debugout();
    QPushButton* testbut;
    QLabel* testlabel;
    void (MainWindow::*tfun[3])(void);

    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void gamestart();
    void reqaddpig();
    void changepage(bool);
    void changeTimeRate();
    void savefile();
    void pac();

    void pageup();
    void pagedown();

    void farmselect0();
    void farmselect1();
    void farmselect2();
    void farmselect3();
    void farmselect4();
    void farmselect5();
    void farmselect6();
    void farmselect7();
    void farmselect8();
    void farmselect9();

    void slottest();
};

#endif // MAINWINDOW_H
