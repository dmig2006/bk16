#include "tracking.h"
#include "controllermanager.h"

Tracking::Tracking()
{
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    res_x_1 = 0;
    res_y_1 = 0;
    res_x_2 = 0;
    res_y_2 = 0;
    mAmendmentBallistX = 0;
    mAmendmentBallistY = 0;
    mWeaponType = WEAPON_KORD;
    clock_gettime(CLOCK_MONOTONIC, &time_s_x);
    clock_gettime(CLOCK_MONOTONIC, &time_s_y);
    h_x = 0.0;
    h_y = 0.0;
}

/*установка позиции по горизонтали и времени опорной точки*/
void Tracking::setPositionsX(long x, float res_x)
{
    mMutex.lock();
    clock_gettime(CLOCK_MONOTONIC, &time_n_x);
    h_x += ((double)(time_n_x.tv_sec) + (double)(time_n_x.tv_nsec) / 1000000000)
           - ((double)(time_s_x.tv_sec) + (double)(time_s_x.tv_nsec) / 1000000000);
    time_s_x = time_n_x;
    if (h_x > 1.1) {
        mTimePositionX = h_x;
        x1 = x2;
        x2 = x;
        res_x_1 = res_x_2;
        res_x_2 = res_x;
        h_x = 0.0;
    }
    mMutex.unlock();
}

/*установка позиции по вертикали и времени опорной точки*/
void Tracking::setPositionsY(long y, float res_y)
{
    mMutex.lock();
    clock_gettime(CLOCK_MONOTONIC, &time_n_y);
    h_y += ((double)(time_n_y.tv_sec) + (double)(time_n_y.tv_nsec) / 1000000000)
           - ((double)(time_s_y.tv_sec) + (double)(time_s_y.tv_nsec) / 1000000000);
    time_s_y = time_n_y;
    if (h_y > 1.1) {
        mTimePositionY = h_y;
        y1 = y2;
        y2 = y;
        res_y_1 = res_y_2;
        res_y_2 = res_y;
        h_y = 0.0;
    }
    mMutex.unlock();
}

/*установка времени стрельбы*/
void Tracking::setTimeSpeed(float p_time_speed)
{
    mMutex.lock();
    m_time_speed = p_time_speed;
    mMutex.unlock();
}

/*установка дальности*/
void Tracking::setRange(int pRange)
{
    mMutex.lock();
    mRange = pRange;
    mMutex.unlock();
}

/*установка типа вооружения*/
void Tracking::setWeaponType(WeaponType pWeaponType)
{
    mMutex.lock();
    mWeaponType = pWeaponType;
    mMutex.unlock();
}

/*установка баллистической поправки*/
void Tracking::setAmendmentBallist(long x, long y)
{
    mMutex.lock();
    mAmendmentBallistX = x;
    mAmendmentBallistY = y;
    mMutex.unlock();
}

/*установка рассогласования рамки сопровождения и основного перекрестия*/
void Tracking::setDelta(long x, long y)
{
    mMutex.lock();
    mDeltaX = x;
    mDeltaY = y;
    mMutex.unlock();
}

/*рассчет точки встречи с целью*/
bool Tracking::getPosTargets(long &xPos1, long &yPos1, long &vCx, long &vCy)
{
    float x1_deg;
    float x2_deg;
    float y1_deg;
    float y2_deg;
    float t2x;
    float t2y;
    float bullet_time;
    float time_s;
    mMutex.lock();
    t2x = mTimePositionX;
    t2y = mTimePositionY;
    res_x_1 /= DEGREE_TO_CENTIDEGREE;
    res_y_1 /= DEGREE_TO_CENTIDEGREE;
    res_x_2 /= DEGREE_TO_CENTIDEGREE;
    res_y_2 /= DEGREE_TO_CENTIDEGREE;
    x1_deg = static_cast<float>(x1) / DEGREE_TO_CENTIDEGREE + res_x_1;
    x2_deg = static_cast<float>(x2) / DEGREE_TO_CENTIDEGREE + res_x_2;
    y1_deg = static_cast<float>(y1) / DEGREE_TO_CENTIDEGREE + res_y_1;
    y2_deg = static_cast<float>(y2) / DEGREE_TO_CENTIDEGREE + res_y_2;
    bullet_time = CfgManager::getSingletonPtr()->getTime(mWeaponType, mRange);
    time_s = m_time_speed;
    //смещение исходя из рассогласования координат строба цели и прицельного перекрестия
    float x_cur_strob = static_cast<float>(mDeltaX)
            / DEGREE_TO_CENTIDEGREE + res_x_2;
    float y_cur_strob = static_cast<float>(mDeltaY)
            / DEGREE_TO_CENTIDEGREE + res_y_2;
    // смещения исходя из баллистики
    float x_ballist = static_cast<float>(mAmendmentBallistX)
                      / DEGREE_TO_CENTIDEGREE;
    float y_ballist = static_cast<float>(mAmendmentBallistY)
            / DEGREE_TO_CENTIDEGREE;
    //смещение цели за время полёта тела
    float  x_target = (x2_deg - x1_deg) / t2x * bullet_time;
    float  y_target = (y2_deg - y1_deg) / t2y * bullet_time;
    //упреждение по осям
    //rN1, rN2 задаётся в метрах, пересчитать исходя из заданной дальности в углы
    float rN1x = 0;
    float rN1y = 0;
    float rN2x = 0;
    float rN2y = 0;

    if (fabs(x2_deg - x1_deg) > DEGREES_EPS) {
        rN1x= atan2f(rN1, mRange) * 180.0f / M_PI
              * sinf(atan2f(x2_deg - x1_deg, y2_deg - y1_deg));
        rN2x= atan2f(rN2, mRange) * 180.0f / M_PI
              * sinf(atan2f(x2_deg - x1_deg, y2_deg - y1_deg));
    }
    if (fabs(y2_deg - y1_deg) > DEGREES_EPS) {
        rN1y= atan2f(rN1, mRange) * 180.0f / M_PI
              * cosf(atan2f(x2_deg - x1_deg, y2_deg - y1_deg));
        rN2y= atan2f(rN2, mRange) * 180.0f / M_PI
              * cosf(atan2f(x2_deg - x1_deg, y2_deg - y1_deg));
    }
    mMutex.unlock();

    float tVx = (fabs(x_target + x_ballist + rN1x + x_cur_strob))
                / MAX_SPEED_MOTORS;
    float tVy = (fabs(y_target + y_ballist + rN1y + y_cur_strob))
                / MAX_SPEED_MOTORS;
    if (tVx < 0 || tVy < 0)
        return false;

    float max_time = 0;
    if (tVx > tVy)
        max_time = tVx;
    else
        max_time = tVy;
    x_target = (x2_deg - x1_deg) * (bullet_time + max_time + tEx) / t2x
               + x_ballist + rN1x + x_cur_strob;
    y_target = (y2_deg - y1_deg) * (bullet_time + max_time + tEy) / t2y
               + y_ballist + rN1y + y_cur_strob;

    if ((x_target > X_BORDER_MAX) || (x_target < X_BORDER_MIN))
        return false;

    float y_position;
    y_position = static_cast<float>(y2) / DEGREE_TO_CENTIDEGREE;

    if ((y_position + y_target > Y_BORDER_MAX)
            || (y_position + y_target < Y_BORDER_MIN))
        return false;

    xPos1 = static_cast<long>(x_target * DEGREE_TO_CENTIDEGREE);
    yPos1 = static_cast<long>(y_target * DEGREE_TO_CENTIDEGREE);
    vCx = static_cast<long>(-((x2_deg - x1_deg) / t2x - (rN1x + rN2x) / time_s)
            * 425.0 / 6.0);
    vCy = -static_cast<long>(((y2_deg - y1_deg) / t2y - (rN1y + rN2y) / time_s)
            * 512.5 / 6.0);

    if (fabs(x2_deg - x1_deg) < DEGREES_EPS)
        vCx = 0;
    if (fabs(y2_deg - y1_deg) < DEGREES_EPS)
        vCy = 0;
    return true;
}
