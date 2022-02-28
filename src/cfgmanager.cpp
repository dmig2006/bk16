#include "cfgmanager.h"

CfgManager* CfgManager::mCfgManager = 0;

CfgManager::CfgManager()
{
    mTcpCfg = 0;
    mJoystickCfg = 0;
    mIDLog = 0;
    loadCfg();
    mTableFileKORD = SupportManager::getSingletonPtr()->loadTable(FILE_TABLE_KORD,
                                                                  WEAPON_KORD);
    mTableFilePKT = SupportManager::getSingletonPtr()->loadTable(FILE_TABLE_PKT,
                                                                 WEAPON_PKT);
    mTableFileAGS = SupportManager::getSingletonPtr()->loadTable(FILE_TABLE_AGS,
                                                                 WEAPON_AGS);
    mTableFileAG40 = SupportManager::getSingletonPtr()->loadTable(FILE_TABLE_AG40,
                                                                  WEAPON_AG40);
}

/*закрузка настроек джойстика и юстировки*/
void CfgManager::loadCfg()
{
    SupportManager *sm = SupportManager::getSingletonPtr();
    //Configuration Parameters
    TiXmlDocument docTcpCfg = sm->loadFileXml(MBDU_CFG_FILE_NETWORK);
    TiXmlDocument docJoystickCfg = sm->loadFileXml(MBDU_CFG_FILE_JOYSTICK);

    mTcpCfg = new TcpCfg();
    mTcpCfg->maddr = sm->getValueXml("address", docTcpCfg);
    mTcpCfg->mPort = atoi(sm->getValueXml("port", docTcpCfg ).c_str());

    mJoystickCfg = new JoystickCfg();
    mJoystickCfg->mMaxXleft  = atoi(sm->getValueXml("max_x_left", docJoystickCfg).c_str());
    mJoystickCfg->mMaxXright = atoi(sm->getValueXml("max_x_right", docJoystickCfg).c_str());
    mJoystickCfg->mMaxYdown  = atoi(sm->getValueXml("max_y_down", docJoystickCfg).c_str());
    mJoystickCfg->mMaxYup    = atoi(sm->getValueXml("max_y_up", docJoystickCfg).c_str());
    mJoystickCfg->mAxisX     = atoi(sm->getValueXml("axis_x", docJoystickCfg).c_str());
    mJoystickCfg->mAxisY     = atoi(sm->getValueXml("axis_y", docJoystickCfg).c_str());
    mJoystickCfg->mWCamera   = atoi(sm->getValueXml("W_CAMERA", docJoystickCfg).c_str());
    mJoystickCfg->mHCamera   = atoi(sm->getValueXml("H_CAMERA", docJoystickCfg).c_str());
    mJoystickCfg->mTCamera   = atoi(sm->getValueXml("T_CAMERA", docJoystickCfg).c_str());
    mJoystickCfg->mShot      = atoi(sm->getValueXml("SHOT", docJoystickCfg).c_str());
    mJoystickCfg->mD         = atoi(sm->getValueXml("D", docJoystickCfg).c_str());

    JustFile* lJustFile = sm->loadJust(FILE_JUST);
    mJustCfg[ 0].JustX = lJustFile->mJustValues[ 0];
    mJustCfg[ 0].JustY = lJustFile->mJustValues[ 1];
    mJustCfg[ 1].JustX = lJustFile->mJustValues[ 2];
    mJustCfg[ 1].JustY = lJustFile->mJustValues[ 3];
    mJustCfg[ 2].JustX = lJustFile->mJustValues[ 4];
    mJustCfg[ 2].JustY = lJustFile->mJustValues[ 5];
    mJustCfg[ 3].JustX = lJustFile->mJustValues[ 6];
    mJustCfg[ 3].JustY = lJustFile->mJustValues[ 7];

    mJustCfg[ 4].JustX = lJustFile->mJustValues[ 8];
    mJustCfg[ 4].JustY = lJustFile->mJustValues[ 9];
    mJustCfg[ 5].JustX = lJustFile->mJustValues[10];
    mJustCfg[ 5].JustY = lJustFile->mJustValues[11];
    mJustCfg[ 6].JustX = lJustFile->mJustValues[12];
    mJustCfg[ 6].JustY = lJustFile->mJustValues[13];
    mJustCfg[ 7].JustX = lJustFile->mJustValues[14];
    mJustCfg[ 7].JustY = lJustFile->mJustValues[15];

    mJustCfg[ 8].JustX = lJustFile->mJustValues[16];
    mJustCfg[ 8].JustY = lJustFile->mJustValues[17];
    mJustCfg[ 9].JustX = lJustFile->mJustValues[18];
    mJustCfg[ 9].JustY = lJustFile->mJustValues[19];
    mJustCfg[10].JustX = lJustFile->mJustValues[20];
    mJustCfg[10].JustY = lJustFile->mJustValues[21];
    mJustCfg[11].JustX = lJustFile->mJustValues[22];
    mJustCfg[11].JustY = lJustFile->mJustValues[23];

    mJustCfg[12].JustX = lJustFile->mJustValues[24];
    mJustCfg[12].JustY = lJustFile->mJustValues[25];
    mJustCfg[13].JustX = lJustFile->mJustValues[26];
    mJustCfg[13].JustY = lJustFile->mJustValues[27];
    mJustCfg[14].JustX = lJustFile->mJustValues[28];
    mJustCfg[14].JustY = lJustFile->mJustValues[29];
    mJustCfg[15].JustX = lJustFile->mJustValues[30];
    mJustCfg[15].JustY = lJustFile->mJustValues[31];
}

/*создание только одного экземпляра класса с глобальной точкой доступа*/
CfgManager* CfgManager::getSingletonPtr()
{
    if (!mCfgManager)
        mCfgManager = new CfgManager();
    return mCfgManager;
}

/*получение баллистических данных для соответствующего вооружения*/
TableFile* CfgManager::getAmendment(WeaponType pWeaponType)
{
    switch (pWeaponType) {
    case WEAPON_KORD:
        return mTableFileKORD;

    case WEAPON_PKT:
        return mTableFilePKT;

    case WEAPON_AGS:
        return mTableFileAGS;

    case WEAPON_AG40:
        return mTableFileAG40;

    default:
        return 0;
    }
}

/*сохранение юстировки*/
void CfgManager::setJust(JustCfg pJustCfg[16])
{
    JustFile *lJustFile = new JustFile();
    lJustFile->mJustValues[ 0] = pJustCfg[ 0].JustX;
    lJustFile->mJustValues[ 1] = pJustCfg[ 0].JustY;
    lJustFile->mJustValues[ 2] = pJustCfg[ 1].JustX;
    lJustFile->mJustValues[ 3] = pJustCfg[ 1].JustY;
    lJustFile->mJustValues[ 4] = pJustCfg[ 2].JustX;
    lJustFile->mJustValues[ 5] = pJustCfg[ 2].JustY;
    lJustFile->mJustValues[ 6] = pJustCfg[ 3].JustX;
    lJustFile->mJustValues[ 7] = pJustCfg[ 3].JustY;

    lJustFile->mJustValues[ 8] = pJustCfg[ 4].JustX;
    lJustFile->mJustValues[ 9] = pJustCfg[ 4].JustY;
    lJustFile->mJustValues[10] = pJustCfg[ 5].JustX;
    lJustFile->mJustValues[11] = pJustCfg[ 5].JustY;
    lJustFile->mJustValues[12] = pJustCfg[ 6].JustX;
    lJustFile->mJustValues[13] = pJustCfg[ 6].JustY;
    lJustFile->mJustValues[14] = pJustCfg[ 7].JustX;
    lJustFile->mJustValues[15] = pJustCfg[ 7].JustY;

    lJustFile->mJustValues[16] = pJustCfg[ 8].JustX;
    lJustFile->mJustValues[17] = pJustCfg[ 8].JustY;
    lJustFile->mJustValues[18] = pJustCfg[ 9].JustX;
    lJustFile->mJustValues[19] = pJustCfg[ 9].JustY;
    lJustFile->mJustValues[20] = pJustCfg[10].JustX;
    lJustFile->mJustValues[21] = pJustCfg[10].JustY;
    lJustFile->mJustValues[22] = pJustCfg[11].JustX;
    lJustFile->mJustValues[23] = pJustCfg[11].JustY;

    lJustFile->mJustValues[24] = pJustCfg[12].JustX;
    lJustFile->mJustValues[25] = pJustCfg[12].JustY;
    lJustFile->mJustValues[26] = pJustCfg[13].JustX;
    lJustFile->mJustValues[27] = pJustCfg[13].JustY;
    lJustFile->mJustValues[28] = pJustCfg[14].JustX;
    lJustFile->mJustValues[29] = pJustCfg[14].JustY;
    lJustFile->mJustValues[30] = pJustCfg[15].JustX;
    lJustFile->mJustValues[31] = pJustCfg[15].JustY;
    SupportManager::getSingletonPtr()->saveJust(lJustFile, FILE_JUST);
}

/*получение юстировки для соответствюхи камер и вооружения*/
JustCfg CfgManager::getJust(int pCameraMode, int pWeaponType)
{
    switch (pCameraMode) {
    case 1:
        return mJustCfg[pWeaponType * 4 + 0];
    case 2:
        return mJustCfg[pWeaponType * 4 + 1];
    case 3:
        return mJustCfg[pWeaponType * 4 + 2];
    case 4:
        return mJustCfg[pWeaponType * 4 + 3];
    }
    return mJustCfg[0];
}

/*получение времени полёта выстрела*/
float CfgManager::getTime(WeaponType pWeaponType, int pRange)
{   
    int num = 0;
    int tmp_range = 10000;
    float balkan_time=1;

    if (pWeaponType != WEAPON_AGS) {
       if (pWeaponType == WEAPON_AG40)
            balkan_time = pRange / 255;
        else{
            for (int i = 0; i < 21; ++i)
                if (abs(pRange - i * 100) <= tmp_range) {
                    tmp_range = abs(pRange - i * 100);
                    num = i;
                    if (tmp_range == 0)
                        break;
                }
            }
    } else {

        for (int i = 0; i <= 35; ++i)
            if (abs(pRange - i * 50) <= tmp_range) {
                tmp_range = abs(pRange - i * 50);
                num = i;
                if (tmp_range == 0)
                    break;
            }
    }

    switch (pWeaponType) {
    case WEAPON_KORD:
        return cB32Times[num];
    case WEAPON_PKT:
        return cM80Times[num];
    case WEAPON_AGS:
        return AG17Times[num];
    case WEAPON_AG40:
        return balkan_time;
    default:
        return cB32Times[num];
    }
}
