#include "ballistics.h"

/*расчет баллистической поправки*/
bool Ballistics::calculate(WeaponType pWeaponType, long pRange, long pThermo,
                           long pPressure, long pWindX, long pWindY)
{
    TableFile *bal_table = CfgManager::getSingletonPtr()->getAmendment(pWeaponType);
    if (!bal_table) {
        std::cerr << "Ballistic table not found\n";
        return false;
    }

    mAmendmentX = mAmendmentY = 0;

    if (pRange < 100)
        return true;

    double kx = DEGREE_TO_CENTIDEGREE;
    double ky = DEGREE_TO_CENTIDEGREE;
    double my = ARCMINUTE_TO_CENTIDEGREE;
    long tmp_range = 0;
    long xi1;
    long xi2;
    long yi1;
    long yi2;
    double a;
    double b;
    int num = 0;

    if (pRange > 2000)
        pRange = 2000;

    if (pThermo < -40)
        pThermo = -40;
    if (pThermo > 60)
        pThermo = 60;

    if (pPressure < 400)
        pPressure = 400;
    if (pPressure > 800)
        pPressure = 800;

    switch (pWeaponType) {
    case WEAPON_KORD:   /* Calculate Amendment for KORD */
        {
        tmp_range = 10000;
        for (int i = 0; i < 20; ++i) {
            long tmp = abs(pRange - bal_table->Range[i]);
            if (tmp <= tmp_range) {
                tmp_range = tmp;
                num = i;
                if (!tmp_range)
                    break;
            }
        }

        if (pWindX < -9)
            mAmendmentX += lrint(-0.06 * kx * bal_table->CrossWind10[num]);
        else if (pWindX >= -9 && pWindX < -7)
            mAmendmentX += lrint(-0.06 * kx * bal_table->CrossWind8[num]);
        else if (pWindX >= -7 && pWindX < -5)
            mAmendmentX += lrint(-0.06 * kx * bal_table->CrossWind6[num]);
        else if (pWindX >= -5 && pWindX < -2)
            mAmendmentX += lrint(-0.06 * kx * bal_table->CrossWind4[num]);
        else if (pWindX >= -2 && pWindX <= 2)
            mAmendmentX = 0;
        else if (pWindX > 2 && pWindX <= 5)
            mAmendmentX += lrint(0.06 * kx * bal_table->CrossWind4[num]);
        else if (pWindX > 5 && pWindX <= 7)
            mAmendmentX += lrint(0.06 * kx * bal_table->CrossWind6[num]);
        else if (pWindX > 7 && pWindX <= 9)
            mAmendmentX += lrint(0.06 * kx * bal_table->CrossWind8[num]);
        else
            mAmendmentX += lrint(0.06 * kx * bal_table->CrossWind10[num]);
        mAmendmentX += lrint(0.06 * kx * bal_table->Derivation[num]);

        if (pWindY < -7)
            mAmendmentY = lrint(-ky * atan(bal_table->LongitudinalWind10[num]
                                           / (double)bal_table->Range[num])
                                * 180.0 / M_PI);
        else if (pWindY >= -6 && pWindY < -2)
            mAmendmentY = lrint(-ky * atan(bal_table->LongitudinalWind4[num]
                                           / (double)bal_table->Range[num])
                                * 180.0 / M_PI);
        else if (pWindY >= -2 && pWindY <= 2)
            mAmendmentY = 0;
        else if (pWindY > 2 && pWindY <= 7)
            mAmendmentY = lrint(ky * atan(bal_table->LongitudinalWind4[num]
                                          / (double)bal_table->Range[num])
                                * 180.0 / M_PI);
        else 
            mAmendmentY = lrint(ky * atan(bal_table->LongitudinalWind10[num]
                                          / (double)bal_table->Range[num])
                                * 180.0 / M_PI);

        yi1 = (pPressure - 750) * bal_table->Pressure[num] / 10;
        yi2 = (15 - pThermo) * bal_table->Thermo[num] / 10;

        tmp_range = 10000;
        for (int i = 0; i < 20; ++i) {
            long tmp = abs(pRange + yi1 + yi2 - bal_table->Range[i]);
            if (tmp <= tmp_range) {
                tmp_range = tmp;
                num = i;
                if (tmp_range == 0)
                    break;
            }
        }

        mAmendmentY += lrint((double)bal_table->Gradus[num] * ky
                             + (double)bal_table->Minuta[num] * my);
        }
        break;

    case WEAPON_PKT:
    {
    if (pRange > 2000)
        pRange = 2000;

    for (int i = 0; i < 20; ++i) {
        if (bal_table->Range[i] - pRange > 0) {
            num = i;
            break;
        }
    }

    if (pRange >= 2000)
        num = 19;

    xi1 = bal_table->Range[num - 1];
    xi2 = bal_table->Range[num];
    yi1 = (double)bal_table->Gradus[num - 1] * ky
          + (double)bal_table->Minuta[num - 1] * my;
    yi2 = (double)bal_table->Gradus[num] * ky
          + (double)bal_table->Minuta[num] * my;
    a = (double)(yi2 - yi1) / (double)(xi2 - xi1);
    b = yi1 - a * (double)xi1;
    mAmendmentY = lrint(a * (double)pRange + b);
    yi1 = kx * atan(bal_table->Wind[num - 1]
                    / (double)bal_table->Range[num - 1]) * 180.0 / M_PI;
    yi2 = kx * atan(bal_table->Wind[num]
                    / (double)bal_table->Range[num]) * 180.0 / M_PI;
    a = (double)(yi2 - yi1) / (double)(xi2 - xi1);
    b = yi1 - a * (double)xi1;
    mAmendmentX = lrint(a * (double)pRange + b);

    if (pWindX < -6)
        mAmendmentX *= -2;
    else if (pWindX >= -6 && pWindX < -4)
        mAmendmentX *= -1;
    else if (pWindX >= -4 && pWindX < -2)
        mAmendmentX /= -2;
    else if (pWindX >= -2 && pWindX <= 2)
        mAmendmentX = 0;
    else if (pWindX > 2 && pWindX <= 4)
        mAmendmentX /= 2;
    else if (pWindX > 6)
        mAmendmentX *= 2;
    }
    break;

    case WEAPON_AGS:
        {
        if (pRange > 1730)
            pRange = 1730;

        tmp_range = 10000;
        for (int i = 0; i < 35; ++i) {
            long tmp = abs(pRange - bal_table->Range[i]);
            if (tmp <= tmp_range) {
                tmp_range = tmp;
                num = i;
                if (!tmp_range)
                    break;
            }
        }

        mAmendmentX = lrint(-0.06 * kx * bal_table->CrossWind[num]);
        if (pWindX < -6)
            mAmendmentX *= -1;
        else if (pWindX >= -6 && pWindX < -4)
            mAmendmentX /= -2;
        else if (pWindX >= -4 && pWindX <= 4)
            mAmendmentX = 0;
        else if (pWindX > 4 && pWindX <= 6)
            mAmendmentX /= 2;
        mAmendmentX += lrint(-0.06 * kx * bal_table->Derivation[num]);

        mAmendmentY = lrint(-ky * 180.0 / M_PI * atan(bal_table->LongitudinalWind[num]
                                                      / (double)bal_table->Range[num]));
        if (pWindY < -6)
            mAmendmentY *= -1;
        else if (pWindY >= -6 && pWindY < -4)
            mAmendmentY /= -2;
        else if (pWindY >= -4 && pWindY <= 4)
            mAmendmentY = 0;
        else if (pWindY > 4 && pWindY <= 6)
            mAmendmentY /= 2;

        yi1 = (pPressure - 750) * bal_table->Pressure[num] / 10;
        yi2 = (15 - pThermo) * (bal_table->Thermo[num] + bal_table->ThermoZar[num]) / 10;

        tmp_range = 10000;
        for (int i = 0; i < 35; ++i) {
             long tmp = abs(pRange + yi1 +yi2 - bal_table->Range[i]);
             if (tmp <= tmp_range) {
                 tmp_range = tmp;
                 num = i;
                 if (!tmp_range)
                     break;
             }
        }

        mAmendmentY += lrint((double)bal_table->Gradus[num] * ky
                             + (double)bal_table->Minuta[num] * my);
        }
        break;

    case WEAPON_AG40:
        {
         if (pRange > 2501)
            pRange = 2501;

         for (int i = 0; i < 25; ++i) {
            if (bal_table->Range[i] - pRange > 0) {
                num = i;
                break;
            }
         }

         if (pRange >= 2501)
            num = 24;

         if (num == 0)
            xi1 = 0;
         else
            xi1 = bal_table->Range[num - 1];

         xi2 = bal_table->Range[num];

         if(num == 0)
            yi1 = 0;
         else
            yi1 = (double)bal_table->Gradus[num - 1] * ky
                  + (double)bal_table->Minuta[num - 1] * my;

         yi2 = (double)bal_table->Gradus[num] * ky
               + (double)bal_table->Minuta[num] * my;

         a = (double)(yi2 - yi1) / (double)(xi2 - xi1);
         b = yi1 - a * (double)xi1;
         mAmendmentY = lrint(a * (double)pRange + b);
        }
        break;


        default:
        break;
    }

    return true;
}
