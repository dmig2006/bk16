 #ifndef AIMLIST_H
#define AIMLIST_H

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <QMutex>
#include <QList>

#include "aim.h"
#ifndef DEGREE_TO_CENTIDEGREE
#define DEGREE_TO_CENTIDEGREE    100.0
#endif

class AimList
{
    static AimList *mAimList;
    QList<Aim*> aims;
    QList<Aim*> sortedAims;
    QMutex mMutex;
    int size_active;

    AimList(){}

public:
    static AimList* getSingletonPtr();
    void push(Aim *pAim);
    Aim* pop();
    void remove(int id);
    void clear();
    void makeAllAimsAlive();
    void sortAims();
    int  getMaxTarget();
    Aim* get(int pId);
    Aim* getByTarget(int id);
    Aim* getAimSortNext(int current_index) const { return sortedAims[current_index]; }
    int  size() const { return aims.size(); }
    int  sizeActive() const { return sortedAims.size(); }
};

#endif // AIMLIST_H
