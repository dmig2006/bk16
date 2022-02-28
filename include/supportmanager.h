#ifndef SUPPORT_MANAGER_H
#define SUPPORT_MANAGER_H
/*****************************************************************************/
/*** Support manager                                                       ***/
/*****************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>
#include <unistd.h>

//support xml files config-----------------------------------------------------
#include "tinyxml.h"
#include "tinystr.h"

#include "weapon.enum.h"

#define FILE_TABLE_KORD "config/ballistic_tables/kord.btb"
#define FILE_TABLE_PKT  "config/ballistic_tables/pkt_2v_205.btb"
#define FILE_TABLE_AGS  "config/ballistic_tables/ags_2v.btb"
#define FILE_TABLE_AG40  "config/ballistic_tables/balkan.btb"

#define FILE_JUST       "config/just.cfg"

const float AG17Times[36] = {
     0.0,  //    0
     0.3,  //   50
     0.5,  //  100
     0.8,  //  150
     1.1,  //  200
     1.4,  //  250
     1.7,  //  300
     2.0,  //  350
     2.3,  //  400
     2.6,  //  450
     3.0,  //  500
     3.3,  //  550
     3.7,  //  600
     4.1,  //  650
     4.5,  //  700
     4.9,  //  750
     5.3,  //  800
     5.7,  //  850
     6.1,  //  900
     6.5,  //  950
     7.0,  // 1000
     7.5,  // 1050
     8.0,  // 1100
     8.5,  // 1150
     9.1,  // 1200
     9.6,  // 1250
    10.0,  // 1300
    11.0,  // 1350
    11.0,  // 1400
    12.0,  // 1450
    13.0,  // 1500
    14.0,  // 1550
    15.0,  // 1600
    16.0,  // 1650
    17.0,  // 1700
    20.0   // 1730
};


const float cB32Times[21] = {
    0.00,  //    0
    0.13,  //  100
    0.26,  //  200
    0.40,  //  300
    0.55,  //  400
    0.70,  //  500
    0.86,  //  600
    1.03,  //  700
    1.21,  //  800
    1.40,  //  900
    1.61,  // 1000
    1.83,  // 1100
    2.06,  // 1200
    2.31,  // 1300
    2.58,  // 1400
    2.86,  // 1500
    3.16,  // 1600
    3.47,  // 1700
    3.79,  // 1800
    4.12,  // 1900
    4.47   // 2000
};

const float cM80Times[21] = {
    0.00,
    0.12,
    0.25,
    0.40,
    0.57,
    0.76,
    0.97,
    1.20,
    1.46,
    1.75,
    2.06,
    2.39,
    2.74,
    3.11,
    3.49,
    3.89,
    4.31,
    4.76,
    5.23,
    5.73,
    6.25
};

struct TableFile {
    int Range[35];
    int Gradus[35];
    int Minuta[35];
    double CrossWind10[35];
    double CrossWind8[35];
    double CrossWind6[35];
    double CrossWind4[35];
    double CrossWind[35];
    double Wind[35];
    double Derivation[35];
    double LongitudinalWind10[35];
    double LongitudinalWind4[35];
    double LongitudinalWind[35];
    double Pressure[35];
    double Thermo[35];
    double ThermoZar[35];
    double AnglePricel[35];
};

struct JustFile {
    long mJustValues[32];
};

class SupportManager
{
    static SupportManager* mSupportManager;

    SupportManager() {}

public:
    static SupportManager* getSingletonPtr();
    //get values
    std::string getValueXml(const char *pNameElement, TiXmlDocument document);
    //loading data
    TiXmlDocument loadFileXml(const char *pFileName);
    TableFile* loadTable(const char *pFileName, WeaponType pWeaponType);
    JustFile* loadJust(const char *pFileName);
    //save data
    bool saveJust(JustFile *lJustFile, const char *pFileName);
};

#endif // SUPPORT_MANAGER_H
