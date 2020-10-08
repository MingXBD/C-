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
    void setGrowRate(float);
    pig* getHead();
    float getGrowRate();
    bool isBlackOnly();
    bool isInfected();
    int killinf();
    int getNumber();
    int getProtectLen();
    int getSize();
    bool isProtected(int);

    bool pigin(pig*);
    int grow();
    void beInfected(int);
    void updateInf();
    float sell(float[3],int);

    void debugout();

    farmblock* next;

    farmblock();
};

#endif // FARMBLOCK_H
