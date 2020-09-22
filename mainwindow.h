#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDebug>
//#include <QFile>
//#include <QTextStream>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QLabel>
#include <QIcon>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
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
    int clicktype;
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
    bool showstat;
    int detailid;
    QString recordname;

    std::vector<farmblock*> heap;
    std::vector<farmblock*> bheap;

    QLabel* labeltime;
    QLabel* labelmoney;
    QLabel* numbar[3];
    QLabel* farmdetail;
    QPushButton* PauseBut;
    QPushButton* blockshow[100];
    QPushButton* StartBut;
    QPushButton* SpeedBut;
    QPushButton* add_pig;
    QPushButton* pagenext;
    QPushButton* pagepre;
    QPushButton* statisticbut;
    QLabel* graphlabel[120];
    QLabel* dback;

    void timerEvent(QTimerEvent *);

    void balanced_insert(pig*&);
    void addpig(int&,int num=50);
    pig* buypig(int&,int);

    void selectfarm(int);
    void showfarmdetail(int);
    void mainmenu();
    void buildblocks();
    void changepage(bool);
    void viewcreate();
    void updatenum();
    void statistic();

    std::fstream f_buy;
    std::ofstream w_save;
    std::ifstream r_save;

    void bchange();
    void gsave();
    void gload();

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
    void pac();

    void showstatistic();

    void slottest();
};

#endif // MAINWINDOW_H
