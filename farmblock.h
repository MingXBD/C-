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
    void grow();
    bool isfull();
    pig* setblackonly(bool);
    bool pigin(pig*);
    void setProtect(int,int);
    void beInfected(int);
    void updateInf();
    float sell(float[3],int);
    void debugout();
    farmblock* next;

    farmblock();
};

#endif // FARMBLOCK_H
