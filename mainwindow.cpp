#include "mainwindow.h"
#include "ui_mainwindow.h"

auto farmcmp=[](farmblock* a,farmblock* b){return *a>*b;};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gamemode=0;

    /*
    testbut=new QPushButton;
    testbut->setGeometry(200,200,50,50);
    testbut->setStyleSheet("QPushButton{border-image: url(:/res/icon.png)}");
    testbut->setParent(this);
    testbut->show();

    ui->label_test1->setText(QString::number(globaltime));
    connect(ui->pushButton_test1,SIGNAL(pressed()),this,SLOT(changeTimeRate()));
    connect(ui->pushButton_test2,SIGNAL(pressed()),this,SLOT(changeicon()));
    */
    debugswitch=0;
    for(int i=0;i<100;i++)
    {
        farms[i].setId(i);
        heap.push_back(&farms[i]);
    }
    make_heap(heap.begin(),heap.end(),farmcmp);
    mainmenu();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    infmon=999999999;
    globaltime++;
    labeltime->setText("Time: "+QString::number(globaltime));
    labelmoney->setText("Money: "+QString::number(money));
    //qDebug()<<globaltime;
    for(int i=0;i<100;i++)
    {
        farms[i].grow();
    }
    if(globaltime%90==0)
    {
        float tmon=0;
        for(int i=0;i<100;i++)
        {
            tmon+=farms[i].sell(prices,globaltime);
        }
        money+=tmon;
    }
}

void MainWindow::mainmenu()
{
    //this->setStyleSheet("QMainWindow{border-image: url(:/res/backgound.png)}");
    StartBut=new QPushButton(this);
    StartBut->setGeometry(200,200,100,100);
    StartBut->setStyleSheet("QPushButton{border-image: url(:/res/start.png)}");
    connect(StartBut,SIGNAL(pressed()),this,SLOT(gamestart()));
}

void MainWindow::debugout()
{
    for(int i=0;i<100;i++)
        farms[i].debugout();
}

pig* MainWindow::buypig(int &mon,int num)
{
    pig* thead=nullptr;
    int totnum=0;
    int tmon=0;
    pig* tp;
    while(totnum<num)
    {
        tp=new pig(qrand()%100<infrate,globaltime);
        totnum++;
        tmon+=prices[tp->getType()]*tp->getWeight();
        if(tmon<mon)
        {
            tp->next=thead;
            thead=tp;
        }
        else
        {
            delete tp;
            break;
        }
    }
    return thead;
}

void MainWindow::balanced_insert(pig *&npig)
{
    pig* leftpig=nullptr;
    while(npig!=nullptr)
    {
        pig* nexp=npig->next;
        if(debugswitch)
        {
            qDebug()<<"balanced_insert: piglist:\n{";
            debuglistout(npig,10);
            qDebug()<<"}";
        }
        if(npig->getType()==0)
        {
            if(bheap.empty()||!bheap[0]->pigin(npig))
            {
                if(heap.empty()||npig->stuck==1||heap[0]->isfull())
                {
                    pig* ttp=npig;
                    npig=npig->next;
                    ttp->next=leftpig;
                    leftpig=ttp;
                    continue;
                }
                npig->stuck=1;
                pig* tp=heap[0]->setblackonly(1);
                bheap.push_back(heap[0]);
                push_heap(bheap.begin(),bheap.end(),farmcmp);
                pop_heap(heap.begin(),heap.end(),farmcmp);
                heap.pop_back();
                if(tp==nullptr)
                    continue;
                pig* tend=tp;
                while(tend->next!=nullptr)
                    tend=tend->next;
                tend->next=npig;
                npig=tp;
            }
            else
            {
                pop_heap(bheap.begin(),bheap.end(),farmcmp);
                push_heap(bheap.begin(),bheap.end(),farmcmp);
                npig=nexp;
            }
        }
        else
        {
            if(heap.empty()||!heap[0]->pigin(npig))
            {
                if(bheap.empty()||npig->stuck==1||bheap[0]->isfull())
                {
                    pig* ttp=npig;
                    npig=npig->next;
                    ttp->next=leftpig;
                    leftpig=ttp;
                    continue;
                }
                npig->stuck=1;
                pig* tp=bheap[0]->setblackonly(0);
                heap.push_back(bheap[0]);
                push_heap(heap.begin(),heap.end(),farmcmp);
                pop_heap(bheap.begin(),bheap.end(),farmcmp);
                bheap.pop_back();
                if(tp==nullptr)
                    continue;
                pig* tend=tp;
                while(tend->next!=nullptr)
                    tend=tend->next;
                tend->next=npig;
                npig=tp;
            }
            else
            {
                pop_heap(heap.begin(),heap.end(),farmcmp);
                push_heap(heap.begin(),heap.end(),farmcmp);
                npig=nexp;
            }
        }
    }
    npig=leftpig;
}

void MainWindow::changeTimeRate()
{
    if(timerate>=10)
        timerate=1;
    else
        timerate++;
    killTimer(globaltimer);
    SpeedBut->setText("Speed x"+QString::number(timerate));
    globaltimer=startTimer(1000/timerate);
}

void MainWindow::gamestart()
{
    globaltime=0;
    timerate=1;
    money=5000;
    infrate=0;
    prices[0]=30;
    prices[1]=14;
    prices[2]=12;
    delete StartBut;

    labeltime=new QLabel(this);
    labeltime->setGeometry(40,30,200,100);
    labeltime->setAlignment(Qt::AlignCenter);
    labeltime->setStyleSheet(
                "QLabel{"
                    "border-image: url(:/res/wood.png);"
                    "color:yellow;"
                    "font:30pt;"
                    "font-weight: bold;"
                "}"
                );
    labeltime->setText("Time: 0");
    labeltime->show();

    labelmoney=new QLabel(this);
    labelmoney->setGeometry(400,30,300,150);
    labelmoney->setAlignment(Qt::AlignCenter);
    labelmoney->setStyleSheet(
                "QLabel{"
                    "border-image: url(:/res/wood.png);"
                    "color:yellow;"
                    "font:20pt;"
                    "font-weight: bold;"
                "}"
                );
    labelmoney->setText("Money: 0");
    labelmoney->show();

    globaltimer=startTimer(1000);
    infmon=999999999;

    pig* piglist=buypig(infmon,qrand()%50-100+100*5);
    balanced_insert(piglist);
    qDebug()<<"insert end";
    debuglistout(piglist,0);
    debugout();

    qDebug()<<"init end";

    add_pig=new QPushButton(this);
    add_pig->setText("add pig");
    add_pig->setGeometry(400,300,100,40);
    add_pig->show();

    connect(add_pig,SIGNAL(pressed()),this,SLOT(addpig()));

    SpeedBut=new QPushButton(this);
    SpeedBut->setText("Speed x1");
    SpeedBut->setGeometry(500,300,100,40);
    SpeedBut->show();

    connect(SpeedBut,SIGNAL(pressed()),this,SLOT(changeTimeRate()));
}

void MainWindow::addpig()
{
    debugswitch=1;
    pig* piglist=buypig(infmon,50);
    balanced_insert(piglist);
    debugout();
    if(piglist==nullptr)
    {
        qDebug()<<"no pig left";
    }
    debuglistout(piglist,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
