#ifndef BALLISTICS_H
#define BALLISTICS_H

#ifndef DEGREE_TO_CENTIDEGREE
#define DEGREE_TO_CENTIDEGREE 100.0
#endif

#ifndef ARCMINUTE_TO_CENTIDEGREE
#define ARCMINUTE_TO_CENTIDEGREE (100.0 / 60.0)
#endif

#include "cfgmanager.h"
#include "weapon.enum.h"

class Ballistics
{
    long mAmendmentX;
    long mAmendmentY;

public:
    Ballistics() : mAmendmentX(0), mAmendmentY(0) {}
    bool calculate(WeaponType pWeaponType, long pRange, long pThermo,
                   long pPressure, long pWindX, long pWindY);
    long getAmendmentX() const { return mAmendmentX; }
    long getAmendmentY() const { return mAmendmentY; }
};

#endif // BALLISTICS_H
