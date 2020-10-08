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

int farmblock::getId()
{
    return id;
}

int farmblock::grow()
{
    float tmon=0;
    pig* tp=head;
    while(tp!=nullptr)
    {
        tp->grow(float(qrand()%13)/10*grow_rate);
        tmon+=grow_rate*(qrand()%10);
        tp=tp->next;
    }
    return tmon;
}

bool farmblock::isfull()
{
    return number>=maxsize;
}

void farmblock::setGrowRate(float t)
{
    grow_rate=t;
}

pig *farmblock::getHead()
{
    return head;
}

float farmblock::getGrowRate()
{
    return grow_rate;
}

bool farmblock::isBlackOnly()
{
    return blackonly;
}

bool farmblock::isInfected()
{
    return infected;
}

int farmblock::killinf()
{
    int num=0;
    pig* pre=nullptr;
    pig* tp=head;
    while(tp!=nullptr)
    {
        if(tp->isInfected())
        {
            number--;
            num++;
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
    this->infected=0;
    return num;
}

int farmblock::getNumber()
{
    return number;
}

int farmblock::getProtectLen()
{
    return protectlen;
}

int farmblock::getSize()
{
    return maxsize;
}

bool farmblock::isProtected(int time)
{
    if(protectlen>=time)
        return true;
    return false;
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
    if(protectlen>=time||number==0)
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
    pig* pre=nullptr;
    while(tp!=nullptr)
    {
        if(tp->isInfected())
        {
            if(tp->isDead())
            {
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
        else
        {
            if(qrand()%100<50)
                tp->beInfected();
            pre=tp;
            tp=tp->next;
        }
    }
    if(number==0)
        infected=0;
}

float farmblock::sell(float p[3],int time)
{
    float tot=0;
    pig* pre=nullptr;
    pig* tp=head;
    while(tp!=nullptr)
    {
        if(!tp->isInfected()&&(time-(tp->getBuytime())>=365||tp->getWeight()>=75))
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
