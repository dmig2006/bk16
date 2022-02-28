#include "aimlist.h"

AimList* AimList::mAimList = 0;

/*создание только одного экземпляра класса с глобальной точкой доступа*/
AimList* AimList::getSingletonPtr()
{
    if (!mAimList)
        mAimList = new AimList();
    return mAimList;
}

/*добавление цели в список*/
void AimList::push(Aim *pAim)
{
    mMutex.lock();
    aims.push_back(pAim);
    mMutex.unlock();
}

/*извлечение цели из списка*/
Aim* AimList::pop()
{
    mMutex.lock();
    if (aims.size() > 0) {
        Aim *lAim = aims.front();
        aims.pop_front();
        mMutex.unlock();
        return lAim;
    } else {
        mMutex.unlock();
        return 0;
    }
}

/*удаление цели из списка*/
void AimList::remove(int id)
{
    mMutex.lock();
    if (aims.size() > 0) {
        aims.removeOne(aims[id]);
        mMutex.unlock();
    } else {
        mMutex.unlock();
    }
}

/*очистка списка целей*/
void AimList::clear()
{
    mMutex.lock();
    aims.clear();
    mMutex.unlock();
}

/*маркировка всех целей необработанными*/
void AimList::makeAllAimsAlive()
{
    mMutex.lock();
    for (int i = 0; i < aims.size(); ++i)
        aims[i]->isAlive = true;
    mMutex.unlock();
}

/*формирование списка целей по горизонтальной координате*/
void AimList::sortAims()
{
    Aim *lAimBuffer = 0;
    sortedAims.clear();

    for (int i = 0; i < aims.size(); ++i) {
        if (aims[i]->mActive)
            sortedAims.push_back(aims[i]);
    }

    for (int i = sortedAims.size() - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            int range_1 = 0;
            int range_2 = 0;
            if (fmod((double)sortedAims[j]->mPositionX,
                     360 * DEGREE_TO_CENTIDEGREE) >= 180 * DEGREE_TO_CENTIDEGREE) {
                range_1 = fmod((double)sortedAims[j]->mPositionX,
                               360 * DEGREE_TO_CENTIDEGREE)
                          - 360 * DEGREE_TO_CENTIDEGREE;
            } else {
                range_1 = fmod((double)sortedAims[j]->mPositionX,
                               360 * DEGREE_TO_CENTIDEGREE);
            }

            if (fmod((double)sortedAims[j + 1]->mPositionX,
                     360 * DEGREE_TO_CENTIDEGREE) >= 180 * DEGREE_TO_CENTIDEGREE) {
                range_2 = fmod((double)sortedAims[j + 1]->mPositionX,
                               360 * DEGREE_TO_CENTIDEGREE)
                          - 360 * DEGREE_TO_CENTIDEGREE;
            } else {
                range_2 = fmod((double)sortedAims[j + 1]->mPositionX,
                               360 * DEGREE_TO_CENTIDEGREE);
            }
            if (range_1 >= range_2) {
                lAimBuffer = sortedAims[j];
                sortedAims[j] = sortedAims[j + 1];
                sortedAims[j + 1] = lAimBuffer;
            }
        }
    }
}

/*получение максимального номера цели*/
int  AimList::getMaxTarget()
{
    int lTarget = 0;
    mMutex.lock();
    for (int i = 0; i < aims.size(); ++i)
        if (aims[i]->mTarget > lTarget)
            lTarget = aims[i]->mTarget;
    mMutex.unlock();
    return lTarget;
}

/*получение цели по индексу списка*/
Aim* AimList::get(int pId)
{
    mMutex.lock();
    Aim *lAim = 0;
    if (pId >= 0 && pId < aims.size())
        lAim = aims[pId];
    mMutex.unlock();
    return lAim;
}

/*получение цели по номеру цели*/
Aim* AimList::getByTarget(int pTarget)
{
    mMutex.lock();
    Aim *lAim = 0;
    for (int i = 0; i < aims.size(); ++i)
        if (aims[i]->mTarget == pTarget && lAim->mActive) {
            lAim = aims[i];
            break;
        }
    mMutex.unlock();
    return lAim;
}
