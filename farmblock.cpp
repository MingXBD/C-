#include "farmblock.h"
#include <QTime>

bool farmblock::operator <(const farmblock a)
{
    return number<a.number;
}

bool farmblock::operator >(const farmblock a)
{
    return number>a.number;
}

void farmblock::setId(int nn)
{
    if(id==-1)
        id=nn;
}

void farmblock::grow()
{
    pig* tp=head;
    while(tp!=nullptr)
    {
        tp->grow(float(qrand()%13)/10*grow_rate);
        tp=tp->next;
    }
}

bool farmblock::isfull()
{
    return number>=maxsize;
}

pig *farmblock::setblackonly(bool nbo)
{
    if(nbo==blackonly)
        return nullptr;
    pig* tp=head;
    head=nullptr;
    number=0;
    infected=0;
    blackonly=nbo;
    return tp;
}

bool farmblock::pigin(pig *np)
{
    if(number>=maxsize||(np->getType()==0&&blackonly==0)||(np->getType()!=0&&blackonly==1))
    {
        return false;
    }
    np->stuck=0;
    np->next=head;
    head=np;
    number++;
    if(np->isInfected())
        infected=1;
    return true;
}

void farmblock::setProtect(int len,int time)
{
    protectlen=time+len;
}

void farmblock::beInfected(int time)
{
    if(protectlen>=time)
        return;
    int tn=qrand()%number;
    pig* tp=head;
    for(int i=0;i<tn;i++)
        tp=tp->next;
    tp->beInfected();
    infected=1;
}

void farmblock::updateInf()
{
    if(infected==0)
        return;
    pig* tp=head;
    while(tp!=nullptr)
    {
        if(qrand()%100<50)
            tp->beInfected();
        tp=tp->next;
    }
}

float farmblock::sell(float p[3],int time)
{
    float tot=0;
    pig* pre=nullptr;
    pig* tp=head;
    while(tp!=nullptr)
    {
        if(time-tp->getBuytime()>=365||tp->getWeight()>=75)
        {
            tot+=tp->getWeight()*p[tp->getType()];
            number--;
            if(tp==head)
            {
                head=tp->next;
                delete tp;
                tp=head;
            }
            else
            {
                pre->next=tp->next;
                delete tp;
                tp=pre->next;
            }
        }
        else
        {
            pre=tp;
            tp=tp->next;
        }
    }
    return tot;
}

void farmblock::debugout()
{
    qDebug()<<"FarmID: "<<id<<' '
            <<"Maxsize: "<<maxsize<<' '
            <<"BlackOnly: "<<(blackonly?"yes":"no")<<' '
            <<"GrowRate: "<<grow_rate<<' '
            <<"ProtectLength: "<<protectlen<<' '
            <<"Infected: "<<(infected?"yes":"no");
    pig* tp=head;
    qDebug()<<"{";
    debuglistout(tp,-1);
    qDebug()<<"}";
}

farmblock::farmblock()
{
    id=-1;
    head=nullptr;
    next=nullptr;
    maxsize=10;
    blackonly=0;
    number=0;
    grow_rate=1;
    protectlen=-1;
    infected=0;
}
