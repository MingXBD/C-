#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDebug>
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
    float prices[3];
    farmblock farms[100];
    int gamemode;

    std::vector<farmblock*> heap;
    std::vector<farmblock*> bheap;

    QLabel* labeltime;
    QLabel* blockshow[10];
    QLabel* labelmoney;
    QPushButton* StartBut;
    QPushButton* SpeedBut;
    QPushButton* add_pig;

    void timerEvent(QTimerEvent *);
    void mainmenu();
    pig* buypig(int&,int);
    void balanced_insert(pig*&);

    int debugswitch;
    void debugout();

    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void gamestart();
    void addpig();
    void changeTimeRate();
};

#endif // MAINWINDOW_H
