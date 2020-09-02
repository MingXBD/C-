#include "pig.h"

int pig::getType()
{
    return type;
}

float pig::getWeight()
{
    return weight;
}

bool pig::isInfected()
{
    return infected;
}

int pig::getBuytime()
{
    return buytime;
}

void pig::grow(float nw)
{
    weight+=nw;
}

void pig::beInfected()
{
    infected=1;
}

void pig::debugout()
{
    qDebug()<<"PigID: "<<id<<' '
            <<"Type: "<<type<<' '
            <<"Weight: "<<weight<<' '
            <<"Infected: "<<(infected?"yes":"no")<<' '
            <<"Buytime: "<<buytime<<' '
            <<"Stuck: "<<stuck;
}

pig::pig(bool isi, int time)
{
    stuck=0;
    id=globalid++;
    type=qrand()%3;
    weight=qrand()%30+20;
    infected=isi;
    buytime=time;
    next=nullptr;
    debugout();
}

int pig::globalid=0;

void debuglistout(pig *tp,int num)
{
    while(tp!=nullptr)
    {
        tp->debugout();
        tp=tp->next;
        if(num--==0)
            break;
    }
}
