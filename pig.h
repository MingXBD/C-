#ifndef PIG_H
#define PIG_H

#include <QTime>
#include <QDebug>

class pig
{
private:
    int type;
    int id;
    static int globalid;
    float weight;
    bool infected;
    int buytime;
    static int deadrate;
public:
    bool stuck;
    int getType();
    float getWeight();
    bool isInfected();
    int getBuytime();
    void grow(float);
    void beInfected();
    bool isDead();
    int getId();

    void debugout();

    pig* next;
    pig(bool, int);
};

void debuglistout(pig*,int);

#endif // PIG_H
