#ifndef FARMBLOCK_H
#define FARMBLOCK_H

#include "pig.h"

class farmblock
{
private:
    int maxsize;
    int number;
    int id;
    pig* head;
    bool blackonly;
    float grow_rate;
    int protectlen;
    bool infected;
public:
    bool operator <(farmblock);
    bool operator >(farmblock);
    void setId(int);
    void setProtect(int,int);
    pig* setblackonly(bool);

    int getId();
    bool isfull();
    pig* getHead();
    int getGrowRate();
    bool isBlackOnly();
    bool isInfected();
    int getNumber();
    int getSize();
    bool isProtected(int);

    bool pigin(pig*);
    void grow();
    void beInfected(int);
    void updateInf();
    float sell(float[3],int);

    void debugout();

    farmblock* next;

    farmblock();
};

#endif // FARMBLOCK_H
