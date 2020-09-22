#include "mainwindow.h"
#include "ui_mainwindow.h"

auto farmcmp=[](farmblock* a,farmblock* b){return *a>*b;};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainmenu();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    globaltime++;

    if(Req_addpig>0)
    {
        while(Req_addpig>0)
        {
            addpig(infmon);
            Req_addpig--;
        }
        add_pig->setText("Add pigs");
    }

    bool ninf[100]={0};
    for(int i=0;i<100;i++)
    {
        if(i-1>=0&&farms[i-1].isInfected()&&qrand()%100<15)
        {
            ninf[i]=1;
        }
        if(i+1<100&&farms[i+1].isInfected()&&qrand()%100<15)
        {
            ninf[i]=1;
        }
    }

    for(int i=0;i<100;i++)
    {
        if(ninf[i])
            farms[i].beInfected(globaltime);
        farms[i].updateInf();
        farms[i].grow();
    }

    make_heap(heap.begin(),heap.end(),farmcmp);
    make_heap(bheap.begin(),bheap.end(),farmcmp);

    if(globaltime%90==0)
    {
        float tmon=0;
        int sellnum=0;
        for(int i=0;i<100;i++)
        {
            int temp=farms[i].getNumber();
            tmon+=farms[i].sell(prices,globaltime);
            sellnum+=temp-farms[i].getNumber();
        }
        money+=tmon;
    }

    if(globaltime%365==0)
        bchange();

    updatenum();

    if(showstat)
        statistic();
    else
        showfarmdetail(detailid);
    labeltime->setText("Time: "+QString::number(globaltime));
    labelmoney->setText("Money: "+QString::number(money));
    buildblocks();
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
    mon-=tmon;

    f_buy<<totnum<<' '<<tmon<<' ';

    return thead;
}

void MainWindow::selectfarm(int id)
{
    switch(clicktype)
    {
    case 1:showfarmdetail(id);break;
    }
}

void MainWindow::showfarmdetail(int id)
{
    if(id==-1)
    {
        farmdetail->setText("No Farm Selected");
        return;
    }
    for(int i=0;i<120;i++)
    {
        graphlabel[i]->hide();
    }
    if(showstat)
    {
        for(int i=0;i<120;i++)
            graphlabel[i]->hide();
        statisticbut->setText("Statistic");
        showstat=0;
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
    int blocknum=page==10?100:10;
    for(int i=0;i<100;i++)
    {
        blockshow[i]->hide();
    }
    for(int i=0;i<blocknum;i++)
    {
        int px,py,pw,ph;
        if(page==10)
        {
            nbut=blockshow[i];
            nfarm=&farms[i];
            pw=40;
            ph=50;
            px=i%10*(pw+20)+100;
            py=i/10*(ph+10)+140;
        }
        else
        {
            nbut=blockshow[page*10+i];
            nfarm=&farms[page*10+i];
            pw=160;
            ph=220;
            px=i%5*(pw+10)+50;
            py=i/5*(ph+10)+200;
        }
        nbut->setGeometry(px,py,pw,ph);

        QString tss="QPushButton{border-image: url(:/res/";
        if(nfarm->getNumber()==0)
        {
            tss+="empty";
        }
        else
        {
            tss+=(nfarm->isInfected()?"inf":"");
            tss+=(nfarm->isBlackOnly()?"black":"normal");
        }
        tss+=".png);font:";
        tss+=QString::number(page==10?13:16);
        tss+="pt;";
        //if(page!=10)
        tss+="font-weight:bold";
        tss+="}";

        nbut->setStyleSheet(tss);
        if(page!=10)
        {
            nbut->setText("FarmId: "+QString::number(nfarm->getId())+"\n"
                          +"Number: "+QString::number(nfarm->getNumber())+"\n"
                          +"GrowSpeed: "+QString::number(nfarm->getGrowRate())
                          );
        }
        else
        {
            nbut->setText(QString::number(nfarm->getNumber())
                          );
        }

        nbut->show();
    }
}

void MainWindow::statistic()
{
    int pignum[2][27]={0};
    int pigtime[13]={0};
    int pigtype[2][3]={0};

    int timeg=15;

    for(int i=0;i<100;i++)
    {
        pig* tp=farms[i].getHead();
        while(tp!=nullptr)
        {
            int temp=(tp->getWeight())*10;
            int isi=(tp->isInfected());

            if(temp>=26*50)
            {
                pignum[isi][26]++;
            }
            else
            {
                int tt=temp/50;
                pignum[isi][tt]++;
            }
            temp=globaltime-(tp->getBuytime());
            if(temp>=12*timeg)
            {
                pigtime[12]++;
            }
            else
            {
                pigtime[temp/timeg]++;
            }
            pigtype[isi][tp->getType()]++;

            tp=tp->next;
        }

    }

    int maxnum=0;
    for(int i=0;i<27;i++)
    {
        int temp=pignum[0][i]+pignum[1][i];
        maxnum=maxnum>temp?maxnum:temp;
    }
    float mw=250;
    float ppx=mw/maxnum<mw/100?mw/maxnum:mw/100;
    //ppx=1;

    int wpx=1000,wpy=30,wph=10,wpg=5;

    for(int i=4;i<27;i++)
    {
        int temp=pignum[1][i]*ppx;
        graphlabel[3*i]->setGeometry(wpx,wpy+i*(wph+wpg),temp,wph);
        graphlabel[3*i]->setStyleSheet("QLabel{"
                                       "border-image: url(:/res/infbar.png)"
                                       "}"
                                       );
        graphlabel[3*i]->setAlignment(Qt::AlignCenter);
        graphlabel[3*i]->setText(QString::number(pignum[1][i]));
        graphlabel[3*i]->show();

        int ttemp=pignum[0][i]*ppx;
        graphlabel[3*i+1]->setGeometry(wpx+temp+1,wpy+i*(wph+wpg),ttemp,wph);
        graphlabel[3*i+1]->setStyleSheet("QLabel{"
                                       "border-image: url(:/res/norbar.png)"
                                         "}"
                                       );
        graphlabel[3*i+1]->setAlignment(Qt::AlignCenter);
        graphlabel[3*i+1]->setText(QString::number(pignum[0][i]));
        graphlabel[3*i+1]->show();

        graphlabel[3*i+2]->setGeometry(wpx-25,wpy+i*(wph+wpg),30,wph);
        graphlabel[3*i+2]->setText(QString::number(float(i*50)/10));
        graphlabel[3*i+2]->show();
    }

    //80 used

    int used=81;

    mw=280;
    updatenum();
    ppx=mw/totpignum;
    wpy=450,wph=20;
    int last=970;
    int bord=last+mw;
    int part=6;

    for(int i=0;i<=12;i++)
    {
        graphlabel[i+used]->setAlignment(Qt::AlignCenter);
        std::string str="";
        str+="QLabel{background-color: #";
        std::stringstream ss;
        ss<< std::uppercase << std::setfill('0') << std::setw(2) << std::hex <<int(196+float(255-196)*(i<=part?i:part)/part);
        std::string tstr;
        ss>>tstr;
        //ss.str("");
        ss.clear();
        str+=tstr;
        ss<< std::uppercase << std::setfill('0') << std::setw(2) << std::hex <<int(250+float(234-250)*(i<=part?i:part)/part);
        ss>>tstr;
        str+=tstr;
        str+="FF}";
        //qDebug()<<QString::fromStdString(str);
        graphlabel[i+used]->setStyleSheet(QString::fromStdString(str));

        int tx=ppx*pigtime[i];
        if(i!=12)
        {
            graphlabel[i+used]->setGeometry(last,wpy,tx,wph);
        }
        else
        {
            if(isPause)
            {
                qDebug()<<"stop here";
            }
            graphlabel[i+used]->setGeometry(last,wpy,bord-last,wph);
        }
        if((i<10&&tx>=5)||(tx>=7))
            graphlabel[i+used]->setText(QString::number(i));
        else
            graphlabel[i+used]->setText("test");

        graphlabel[i+used]->show();
        last+=tx;
    }

    //used 12

    used+=13;
    wpy=480,wph=30;
    last=970;
    bord=last+mw;

    for(int i=0;i<3;i++)
    {
        graphlabel[i+used]->setAlignment(Qt::AlignCenter);
        QString str="";
        str+="QLabel{background-color: #";
        switch(i)
        {
        case 0:str+="EFEF04";break;
        case 1:str+="F7F77A";break;
        case 2:str+="FBFBE3";break;
        }
        str+="}";
        graphlabel[i+used]->setStyleSheet(str);
        graphlabel[i+used]->setGeometry(last,wpy,ppx*pigtype[0][i],wph);
        graphlabel[i+used]->setText(QString::number(pigtype[0][i]));
        graphlabel[i+used]->show();
        last+=ppx*pigtype[0][i];
    }

    used+=3;

    for(int i=2;i>=0;i--)
    {
        graphlabel[i+used]->setAlignment(Qt::AlignCenter);
        QString str="";
        str+="QLabel{background-color: #";
        switch(i)
        {
        case 0:str+="3DF600";break;
        case 1:str+="7AF053";break;
        case 2:str+="BAF4A7";break;
        }
        str+="}";
        graphlabel[i+used]->setStyleSheet(str);
        if(i!=0)
        {
            graphlabel[i+used]->setGeometry(last,wpy,ppx*pigtype[1][i],wph);
        }
        else
        {
            if(isPause)
            {
                qDebug()<<"stop here";
            }
            graphlabel[i+used]->setGeometry(last,wpy,bord-last,wph);
        }
        graphlabel[i+used]->setText(QString::number(pigtype[1][i]));
        graphlabel[i+used]->show();
        last+=ppx*pigtype[1][i];
    }


}

void MainWindow::bchange()
{
    f_buy.close();

    std::ofstream temp;
    std::string name="./rec/Year-"+QString::number(globaltime/365).toStdString()+".txt";
    temp.open(name);
    temp<<"0";
    temp.close();

    f_buy.open(name);
    if(!f_buy.is_open())
        qDebug()<<"f_buy open fail";
}

void MainWindow::gsave()
{

}

void MainWindow::gload()
{

}

void MainWindow::changeTimeRate()
{
    if(timerate>=16)
        timerate=1;
    else
        timerate+=timerate;
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
        statistic();
    }
    isPause=!isPause;
}

void MainWindow::showstatistic()
{
    if(!showstat)
    {
        statisticbut->setText("Close Statistic");
        showstat=1;
        farmdetail->setText("");
        statistic();
    }
    else
    {
        showstat=0;
        for(int i=0;i<120;i++)
            graphlabel[i]->hide();
        showfarmdetail(detailid);
        statisticbut->setText("Statistic");
    }
}

void MainWindow::gamestart()
{

    gamemode=0;
    clicktype=1;

    for(int i=0;i<100;i++)
    {
        blockshow[i]=new QPushButton(this);
        blockshow[i]->hide();
    }
    for(int i=0;i<10;i++)
    {
        blockid[i]=i;
    }

    for(int i=0;i<100;i++)
    {
        farms[i].setId(i);
        heap.push_back(&farms[i]);
    }

    make_heap(heap.begin(),heap.end(),farmcmp);

    isPause=0;
    detailid=-1;
    Req_addpig=0;
    globaltime=0;
    money=500000;
    infrate=0;
    showstat=0;
    prices[0]=30;
    prices[1]=14;
    prices[2]=12;
    delete StartBut;

    viewcreate();

    timerate=1;
    globaltimer=startTimer(1000);
    infmon=999999999;

    addpig(infmon,qrand()%50-100+100*5);

    page=0;
    buildblocks();

    w_save.open("./rec/Year-0.txt");
    w_save<<"0";
    w_save.close();
    f_buy.open("./rec/Year-0.txt");

    for(int i=0;i<100;i++)
    {
        connect(blockshow[i],&QPushButton::pressed,[=](){selectfarm(i);});
    }

    qDebug()<<"init end";

    timerEvent(nullptr);
}

void MainWindow::reqaddpig()
{
    Req_addpig++;
    add_pig->setText("Add pigs ("+QString::number(Req_addpig)+")");
}

void MainWindow::slottest()
{
    pig* tp=new pig(1,globaltime);
    balanced_insert(tp);
    debuglistout(tp,0);
}

void MainWindow::changepage(bool dir)
{
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
}

void MainWindow::viewcreate()
{
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
    connect(pagepre,&QPushButton::pressed,[=](){changepage(0);});

    pagenext=new QPushButton(this);
    pagenext->setGeometry(810,60,pagebutsizew,pagebutsizeh);
    pagenext->setText("Page next ->");
    pagenext->show();
    connect(pagenext,&QPushButton::pressed,this,[=](){changepage(1);});

    dback=new QLabel(this);
    dback->setGeometry(940,0,340,768);
    dback->setStyleSheet(
                "QLabel{"
                "   border-image: url(:/res/board.png);"
                "}");
    dback->show();

    farmdetail=new QLabel(this);
    farmdetail->setGeometry(980,80,300,768);
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
    connect(testbut,SIGNAL(pressed()),this,SLOT(slottest()));

    statisticbut=new QPushButton(this);
    statisticbut->setGeometry(830,100,100,40);
    statisticbut->setText("Statistic");
    statisticbut->show();
    connect(statisticbut,SIGNAL(pressed()),this,SLOT(showstatistic()));

    for(int i=0;i<3;i++)
    {
        numbar[i]=new QLabel(this);
        numbar[i]->setGeometry(640,130,150,60);
        numbar[i]->show();
    }
    numbar[0]->setStyleSheet("QLabel{border-image: url(:/res/norbar.png)}");
    numbar[1]->setStyleSheet("QLabel{border-image: url(:/res/infbar.png)}");
    numbar[2]->setAlignment(Qt::AlignCenter);
    numbar[2]->setStyleSheet("QLabel{font: 10pt;font-weight: bold}");

    for(int i=0;i<120;i++)
    {
        graphlabel[i]=new QLabel(this);
    }

}

void MainWindow::updatenum()
{
    int tnum=0;
    for(int i=0;i<100;i++)
    {
        tnum+=farms[i].getNumber();
    }

    totpignum=tnum;

    numbar[1]->resize(150*float(totpignum)/1000,60);
    numbar[2]->setText(QString::number(totpignum)+"/1000");
}

void MainWindow::addpig(int& mon,int num)
{
    pig* piglist=buypig(mon,num);
    balanced_insert(piglist);
    if(piglist==nullptr)
    {
        f_buy<<"0\n";
        qDebug()<<"no pig left";
    }
    else
    {
        f_buy<<"1\n";
        debuglistout(piglist,0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
