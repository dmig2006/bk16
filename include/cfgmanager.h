#ifndef CFG_MANAGER_H
#define CFG_MANAGER_H
/*****************************************************************************/
/*** cfg                                                                   ***/
/*****************************************************************************/
#include <QList>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cfgfiles.h"
#include "supportmanager.h"
#include "weapon.enum.h"

#define AVATAR_SDELAL

class CfgManager
{
    static CfgManager *mCfgManager;
    TcpCfg *mTcpCfg;
    JoystickCfg *mJoystickCfg;
    TableFile *mTableFileKORD;
    TableFile *mTableFilePKT;
    TableFile *mTableFileAGS;
    TableFile *mTableFileAG40;
    JustCfg mJustCfg[16];
    int mIDLog;
    
    CfgManager();
    void loadCfg();

public:
    static CfgManager* getSingletonPtr();
    TableFile* getAmendment(WeaponType pWeaponType);
    void setJust(JustCfg pJustCfg[12]);
    JustCfg getJust(int pCameraMode, int pWeaponType);
    float getTime(WeaponType pWeaponType, int pRange);
    TcpCfg* getTcpCfg() { return mTcpCfg; }
    JoystickCfg* getJoystickCfg() { return mJoystickCfg; }
};

#endif // CFG_MANAGER_H
