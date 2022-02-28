#ifndef CFGFILES
#define CFGFILES

#include <iostream>
#include <string>

#define MBDU_CFG_FILE_NETWORK  "/home/dogusakov/Designed_project/Module/БК-16/БУ/bk16_v3.1_WORK/resources/res/xml/mbdu_network.xml"
#define MBDU_CFG_FILE_JOYSTICK "/home/dogusakov/Designed_project/Module/БК-16/БУ/bk16_v3.1_WORK/resources/res/xml/mbdu_joystick.xml"

struct JustCfg {
    long JustX;
    long JustY;
};

struct TcpCfg {
    std::string maddr;
    int mPort;
};

struct JoystickCfg {
    int mAxisX;
    int mAxisY;
    int mMaxXleft;
    int mMaxXright;
    int mMaxYup;
    int mMaxYdown;
    int mWCamera;
    int mHCamera;
    int mTCamera;
    int mD;
    int mShot;
};

#endif // CFGFILES_H
