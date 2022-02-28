#ifndef TRACKING_H
#define TRACKING_H

#include <QTime>
#include <QMutex>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//config-----------------------------------------------------------------------
#include "cfgmanager.h"

//support----------------------------------------------------------------------
#include "supportmanager.h"
#include "weapon.enum.h"

#define MAX_SPEED_MOTORS    60
#define MAX_ACC_MOTORS      350

#define DEGREES_EPS         0.02f

#define TIME_EPS            0.01f

#define rN1  1.f
#define rN2  1.f

#define tEx  0.4
#define tEy  0.4

#define X_BORDER_MIN       -70.f
#define X_BORDER_MAX        70.f
#define Y_BORDER_MIN       -20.f
#define Y_BORDER_MAX        70.f

#ifndef DEGREE_TO_CENTIDEGREE
#define DEGREE_TO_CENTIDEGREE   100.0
#endif

class Tracking
{
    float m_time_speed;
    long  x1;
    long  x2;
    long  y1;
    long  y2;
    float  res_x_1;
    float  res_y_1;
    float  res_x_2;
    float  res_y_2;
    int   mRange;
    long  mAmendmentBallistX;
    long  mAmendmentBallistY;
    long  mDeltaX;
    long  mDeltaY;
    float mTimePositionY;
    float mTimePositionX;
    float mV_mot;
    float mA_mot;
    struct timespec time_s_y;
    struct timespec time_n_y;
    struct timespec time_s_x;
    struct timespec time_n_x;
    double h_x;
    double h_y;
    QMutex mMutex;
    WeaponType mWeaponType;

public:
    Tracking();
    void setPositionsX(long x, float res_x);
    void setPositionsY(long y, float res_x);
    void setRange( int pRange );
    void setWeaponType(WeaponType pWeaponType);
    void setAmendmentBallist(long x, long y);
    void setTimeSpeed(float p_time_speed);
    void setDelta(long x, long y);
    bool getPosTargets(long &xPos1, long &yPos1, long &vCx, long &vCy);
};

#endif // TRACKING_H
