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

    for(int i=0;i<100;i++)
    {
        blockshow[i]=nullptr;
    }
    for(int i=0;i<10;i++)
    {
        blockid[i]=i;
    }

    isPause=0;
    detailid=-1;
    debugswitch=0;
    Req_addpig=0;
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
    globaltime++;
    if(Req_addpig>0)
    {
        while(Req_addpig>0)
        {
            addpig();
            Req_addpig--;
        }
        add_pig->setText("Add pigs");
    }
    labeltime->setText("Time: "+QString::number(globaltime));
    labelmoney->setText("Money: "+QString::number(money));
    buildblocks();
    showfarmdetail(detailid);
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
    infmon=999999999;
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

void MainWindow::showfarmdetail(int id)
{
    if(id==-1)
        return;
    if(id==-2)
    {
        statistic();
        return;
    }
    detailid=id;
    QString tstr="Farm :";
    tstr+=QString::number(farms[id].getId())+"\n";
    pig* tp=farms[id].getHead();
    if(tp==nullptr)
    {
        tstr+="No Pig";
    }
    else
    {
        while(tp!=nullptr)
        {
            tstr+="\n";
            tstr+="Pig id: "+QString::number(tp->getId())+"\n";
            tstr+="    Type: ";
            switch(tp->getType())
            {
                case 0:tstr+="Black  ";break;
                case 1:tstr+="Spotted";break;
                case 2:tstr+="White  ";break;
            }
            tstr+="    Weight: "+QString::number(tp->getWeight())+"\n";
            tstr+="    Infected: ";
            tstr+=tp->isInfected()?"Yes":"No ";
            tstr+="    Buytime: "+QString::number(tp->getBuytime());
            tp=tp->next;
        }
    }
    farmdetail->setText(tstr);
}

void MainWindow::balanced_insert(pig *&npig)
{
    pig* leftpig=nullptr;
    if(debugswitch)
    {
        qDebug()<<"balanced_insert: piglist:\n{";
        debuglistout(npig,-1);
        qDebug()<<"}";
        debugswitch=0;
    }
    while(npig!=nullptr)
    {
        pig* nexp=npig->next;
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

void MainWindow::buildblocks()
{
    QPushButton* nbut;
    farmblock* nfarm;
    if(debugswitch)
        debugswitch=0;
    int blocknum=page==10?100:10;
    for(int i=0;i<100;i++)
    {
        if(blockshow[i]!=nullptr)
            blockshow[i]->hide();
    }
    for(int i=0;i<blocknum;i++)
    {
        if(page==10)
        {
            if(blockshow[i]==nullptr)
                blockshow[i]=new QPushButton(this);
            nbut=blockshow[i];
            nfarm=&farms[i];
        }
        else
        {
            if(blockshow[page*10+i]==nullptr)
                blockshow[page*10+i]=new QPushButton(this);
            nbut=blockshow[page*10+i];
            nfarm=&farms[page*10+i];
        }
        int px,py,pw,ph;
        if(page==10)
        {
            pw=60;
            ph=50;
            px=i%10*(pw+10)+100;
            py=i/10*(ph+10)+140;
        }
        else
        {
            pw=150;
            ph=200;
            px=i%5*(pw+10)+10;
            py=i/5*(ph+10)+140;
        }
        nbut->setGeometry(px,py,pw,ph);
        QString tss="QPushButton{border-image: url(:/res/";
        tss+=(nfarm->isBlackOnly()?"":"wood");
        tss+=".png);font:";
        tss+=QString::number(page==10?8:13);
        tss+="pt;";
        if(page!=10)
            tss+="font-weight:bold";
        tss+="}";
        nbut->setStyleSheet(tss);
        if(page!=10)
        {
            nbut->setText("FarmId: "+QString::number(nfarm->getId())+"\n"
                          +"Number: "+QString::number(nfarm->getNumber())+"\n"
                          +"BlackOnly: "+(nfarm->isBlackOnly()?"yes":"no")+"\n"
                          +"GrowSpeed: "+QString::number(nfarm->getGrowRate())+"\n"
                          +"Infected: "+(nfarm->isInfected()?"yes":"no")
                          );
        }
        else
        {
            nbut->setText("Num: "+QString::number(nfarm->getNumber())+"\n"
                          +"Spd: "+QString::number(nfarm->getGrowRate())+"\n"
                          +"Inf: "+(nfarm->isInfected()?"Inf":"Not")
                          );
        }
        nbut->show();
    }
}

void MainWindow::statistic()
{
    int tpignum[2000]={0};
    for(int i=0;i<100;i++)
    {
        pig* tp=farms[i].getHead();
        while(tp!=nullptr)
        {
            tpignum[int((tp->getWeight())*10)]++;
            tp=tp->next;
        }
    }
    QString tstr="Statistic:\n";
    int tjg=50;
    for(int i=0;i<2000;i+=tjg)
    {
        int temp=0;
        for(int j=0;j<tjg;j++)
        {
            temp+=tpignum[i+j];
        }
        tstr+=QString::number(float(i)/10)+" - "+QString::number(float(i+tjg)/10)+" : "+QString::number(temp)+"\n";
    }
    farmdetail->setText(tstr);
    detailid=-2;
}

void MainWindow::showData()
{

}

void MainWindow::savefile()
{
    QFile fsave("test.in");
    if(!fsave.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"open fail";
        return;
    }

    QTextStream tout(&fsave);
    int a;
    tout>>a;
    qDebug()<<a;
    tout>>a;
    qDebug()<<a;
}

void MainWindow::changeTimeRate()
{
    if(timerate>=10)
        timerate=1;
    else
        timerate++;
    if(globaltimer!=-1)
        killTimer(globaltimer);
    SpeedBut->setText("Speed x"+QString::number(timerate));
    if(!isPause)
        globaltimer=startTimer(1000/timerate);
}

void MainWindow::pac()
{
    if(isPause)
    {
        SpeedBut->setText("Speed x"+QString::number(timerate));
        globaltimer=startTimer(1000/timerate);
        PauseBut->setText("Pause");
    }
    else
    {
        PauseBut->setText("Continue");
        killTimer(globaltimer);
        globaltimer=-1;
    }
    isPause=!isPause;
}

void MainWindow::gamestart()
{
    globaltime=0;
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
    labelmoney->setGeometry(400,30,200,100);
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


    add_pig=new QPushButton(this);
    add_pig->setText("Add pigs");
    add_pig->setGeometry(640,10,100,40);
    add_pig->show();
    connect(add_pig,SIGNAL(pressed()),this,SLOT(reqaddpig()));


    SpeedBut=new QPushButton(this);
    SpeedBut->setText("Speed x1");
    SpeedBut->setGeometry(750,10,100,40);
    SpeedBut->show();
    connect(SpeedBut,SIGNAL(pressed()),this,SLOT(changeTimeRate()));

    int pagebutsizew=120,pagebutsizeh=40;

    pagepre=new QPushButton(this);
    pagepre->setGeometry(680,60,pagebutsizew,pagebutsizeh);
    pagepre->setText("<- Page previous");
    pagepre->show();
    connect(pagepre,SIGNAL(pressed()),this,SLOT(pagedown()));

    pagenext=new QPushButton(this);
    pagenext->setGeometry(810,60,pagebutsizew,pagebutsizeh);
    pagenext->setText("Page next ->");
    pagenext->show();
    connect(pagenext,SIGNAL(pressed()),this,SLOT(pageup()));

    farmdetail=new QLabel(this);
    farmdetail->setGeometry(980,0,300,768);
    farmdetail->setText("No Farm Selected");
    farmdetail->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    farmdetail->setStyleSheet(
                "QLabel{"
                "   border-image: url();"
                "   font: 10pt;"
                "   font-weight: bold;"
                "}");
    farmdetail->show();

    PauseBut=new QPushButton(this);
    PauseBut->setGeometry(860,10,70,40);
    PauseBut->setText("Pause");
    PauseBut->show();
    connect(PauseBut,SIGNAL(pressed()),this,SLOT(pac()));

    testbut=new QPushButton(this);
    testbut->setGeometry(0,0,50,50);
    testbut->setText("test");
    testbut->show();
    /*tfun[0]=MainWindow::statistic;
    QObject::connect(testbut,&QPushButton::pressed,[=](){(this->*tfun[0])();});*/
    connect(testbut,SIGNAL(pressed()),this,SLOT(slottest()));

    timerate=1;
    globaltimer=startTimer(1000);
    infmon=999999999;

    debugswitch=0;

    pig* piglist=buypig(infmon,qrand()%50-100+100*5);
    balanced_insert(piglist);
    qDebug()<<"insert end";
    debuglistout(piglist,0);
    //debugout();

    page=0;
    buildblocks();
    connect(blockshow[page*10+0],SIGNAL(pressed()),this,SLOT(farmselect0()));
    connect(blockshow[page*10+1],SIGNAL(pressed()),this,SLOT(farmselect1()));
    connect(blockshow[page*10+2],SIGNAL(pressed()),this,SLOT(farmselect2()));
    connect(blockshow[page*10+3],SIGNAL(pressed()),this,SLOT(farmselect3()));
    connect(blockshow[page*10+4],SIGNAL(pressed()),this,SLOT(farmselect4()));
    connect(blockshow[page*10+5],SIGNAL(pressed()),this,SLOT(farmselect5()));
    connect(blockshow[page*10+6],SIGNAL(pressed()),this,SLOT(farmselect6()));
    connect(blockshow[page*10+7],SIGNAL(pressed()),this,SLOT(farmselect7()));
    connect(blockshow[page*10+8],SIGNAL(pressed()),this,SLOT(farmselect8()));
    connect(blockshow[page*10+9],SIGNAL(pressed()),this,SLOT(farmselect9()));

    qDebug()<<"init end";
}

void MainWindow::reqaddpig()
{
    Req_addpig++;
    add_pig->setText("Add pigs ("+QString::number(Req_addpig)+")");
}

void MainWindow::pageup()
{
    changepage(1);
}

void MainWindow::pagedown()
{
    changepage(0);
}

void MainWindow::farmselect0()
{
    showfarmdetail(blockid[0]);
}

void MainWindow::farmselect1()
{
    showfarmdetail(blockid[1]);
}

void MainWindow::farmselect2()
{
    showfarmdetail(blockid[2]);
}

void MainWindow::farmselect3()
{
    showfarmdetail(blockid[3]);
}

void MainWindow::farmselect4()
{
    showfarmdetail(blockid[4]);
}

void MainWindow::farmselect5()
{
    showfarmdetail(blockid[5]);
}

void MainWindow::farmselect6()
{
    showfarmdetail(blockid[6]);
}

void MainWindow::farmselect7()
{
    showfarmdetail(blockid[7]);
}

void MainWindow::farmselect8()
{
    showfarmdetail(blockid[8]);
}

void MainWindow::farmselect9()
{
    showfarmdetail(blockid[9]);
}

void MainWindow::slottest()
{
    statistic();
}

void MainWindow::changepage(bool dir)
{
    if(page!=10)
    {
        for(int i=0;i<10;i++)
        {
            blockshow[page*10+i]->disconnect();
        }
    }
    if(dir)
        page++;
    else
        page--;
    page=(page%11+11)%11;
    if(page!=10)
    {
        for(int i=0;i<10;i++)
        {
            blockid[i]=page*10+i;
        }
    }
    buildblocks();
    if(page!=10)
    {
        connect(blockshow[page*10+0],SIGNAL(pressed()),this,SLOT(farmselect0()));
        connect(blockshow[page*10+1],SIGNAL(pressed()),this,SLOT(farmselect1()));
        connect(blockshow[page*10+2],SIGNAL(pressed()),this,SLOT(farmselect2()));
        connect(blockshow[page*10+3],SIGNAL(pressed()),this,SLOT(farmselect3()));
        connect(blockshow[page*10+4],SIGNAL(pressed()),this,SLOT(farmselect4()));
        connect(blockshow[page*10+5],SIGNAL(pressed()),this,SLOT(farmselect5()));
        connect(blockshow[page*10+6],SIGNAL(pressed()),this,SLOT(farmselect6()));
        connect(blockshow[page*10+7],SIGNAL(pressed()),this,SLOT(farmselect7()));
        connect(blockshow[page*10+8],SIGNAL(pressed()),this,SLOT(farmselect8()));
        connect(blockshow[page*10+9],SIGNAL(pressed()),this,SLOT(farmselect9()));
    }
}

void MainWindow::addpig()
{
    pig* piglist=buypig(infmon,50);
    balanced_insert(piglist);
    //debugout();
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
