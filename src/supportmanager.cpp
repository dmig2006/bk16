#include "supportmanager.h"

SupportManager* SupportManager::mSupportManager = 0;

/*создание только одного экземпляра класса с глобальной точкой доступа*/
SupportManager* SupportManager::getSingletonPtr()
{
    if (!mSupportManager)
        mSupportManager = new SupportManager();
    return mSupportManager;
}

/*получение элементов файла xml*/
std::string SupportManager::getValueXml(const char *pNameElement, TiXmlDocument document)
{
    TiXmlNode* node     = 0;
    TiXmlElement* todoElement   = 0;
    TiXmlElement* itemElement   = 0;
    //First ToDo element
    node    = document.FirstChild("ConfigurationParameters");
    todoElement = node->ToElement();
    node    = todoElement->FirstChildElement(); // This skips the "PDA" comment.
    itemElement = node->ToElement();
    std::string str = std::string(itemElement->Attribute(pNameElement));
    return str;
}

/*загрузка файла xml*/
TiXmlDocument SupportManager::loadFileXml(const char *pFileName)
{
    TiXmlDocument document(pFileName);
    if (!document.LoadFile()) {
        std::cerr << "XML file: " << pFileName <<  "error\n";
        return 0;
    }
    return document;
}

/*загрузка файла баллистики*/
TableFile* SupportManager::loadTable(const char *pFileName, WeaponType pWeaponType)
{
    TableFile *lTableFile = 0;
    std::fstream file;
    file.open(pFileName, std::ios::in);
    if (!file)
        return 0;
    lTableFile = new TableFile();
    switch (pWeaponType) {
    case WEAPON_PKT:
        for (int i = 0; i < 20; ++i) {
            int range;
            unsigned char degree;
            unsigned char arcminute;
            double wind;

            file.read((char*)&range,     sizeof(range)    );
            file.read((char*)&degree,    sizeof(degree)   );
            file.read((char*)&arcminute, sizeof(arcminute));
            file.read((char*)&wind,      sizeof(wind)     );

            lTableFile->Range[i]              = range;
            lTableFile->Gradus[i]             = degree;
            lTableFile->Minuta[i]             = arcminute;
            lTableFile->Wind[i]               = wind;
            lTableFile->CrossWind10[i]        = 0;
            lTableFile->CrossWind8[i]         = 0;
            lTableFile->CrossWind6[i]         = 0;
            lTableFile->CrossWind4[i]         = 0;
            lTableFile->Derivation[i]         = 0;
            lTableFile->LongitudinalWind10[i] = 0;
            lTableFile->LongitudinalWind4[i]  = 0;
            lTableFile->Pressure[i]           = 0;
            lTableFile->Thermo[i]             = 0;
        }
        break;
    case WEAPON_KORD:
        for (int i = 0; i < 20; ++i) {
            int range;
            unsigned char degree;
            unsigned char arcminute;
            unsigned char pressure;
            unsigned char temperature;
            unsigned char headwind_4;
            unsigned char headwind_10;
            double crosswind_4;
            double crosswind_6;
            double crosswind_8;
            double crosswind_10;
            double derivation;

            file.read((char*)&range,        sizeof(range)       );
            file.read((char*)&degree,       sizeof(degree)      );
            file.read((char*)&arcminute,    sizeof(arcminute)   );
            file.read((char*)&pressure,     sizeof(pressure)    );
            file.read((char*)&temperature,  sizeof(temperature) );
            file.read((char*)&headwind_4,   sizeof(headwind_4)  );
            file.read((char*)&headwind_10,  sizeof(headwind_10) );
            file.read((char*)&crosswind_4,  sizeof(crosswind_4) );
            file.read((char*)&crosswind_6,  sizeof(crosswind_6) );
            file.read((char*)&crosswind_8,  sizeof(crosswind_8) );
            file.read((char*)&crosswind_10, sizeof(crosswind_10));
            file.read((char*)&derivation,   sizeof(derivation)  );

            lTableFile->Range[i]              = range;
            lTableFile->Gradus[i]             = degree;
            lTableFile->Minuta[i]             = arcminute;
            lTableFile->Wind[i]               = 0;
            lTableFile->CrossWind10[i]        = crosswind_10;
            lTableFile->CrossWind8[i]         = crosswind_8;
            lTableFile->CrossWind6[i]         = crosswind_6;
            lTableFile->CrossWind4[i]         = crosswind_4;
            lTableFile->Derivation[i]         = derivation;
            lTableFile->LongitudinalWind10[i] = headwind_10;
            lTableFile->LongitudinalWind4[i]  = headwind_4;
            lTableFile->Pressure[i]           = pressure;
            lTableFile->Thermo[i]             = temperature;
        }
        break;
    case WEAPON_AGS:
        for (int i = 0; i < 35; ++i) {
            int range;
            unsigned char degree;
            unsigned char arcminute;
            unsigned char derivation;
            unsigned char crosswind;
            unsigned char headwind;
            unsigned char pressure;
            unsigned char temperature;
            unsigned char z_temperature;
            double aiming_angle;

            file.read((char*)&range,         sizeof(range)        );
            file.read((char*)&degree,        sizeof(degree)       );
            file.read((char*)&arcminute,     sizeof(arcminute)    );
            file.read((char*)&derivation,    sizeof(derivation)   );
            file.read((char*)&crosswind,     sizeof(crosswind)    );
            file.read((char*)&headwind,      sizeof(headwind)     );
            file.read((char*)&pressure,      sizeof(pressure)     );
            file.read((char*)&temperature,   sizeof(temperature)  );
            file.read((char*)&z_temperature, sizeof(z_temperature));
            file.read((char*)&aiming_angle,  sizeof(aiming_angle) );

            lTableFile->Range[i]            = range;
            lTableFile->Gradus[i]           = degree;
            lTableFile->Minuta[i]           = arcminute;
            lTableFile->Derivation[i]       = derivation;
            lTableFile->CrossWind[i]        = crosswind;
            lTableFile->LongitudinalWind[i] = headwind;
            lTableFile->Pressure[i]         = pressure;
            lTableFile->Thermo[i]           = temperature;
            lTableFile->ThermoZar[i]        = z_temperature;
            lTableFile->AnglePricel[i]      = aiming_angle;
        };
        break;
 case WEAPON_AG40:
        for (int i = 0; i < 25; ++i) {
            int range;
            unsigned char degree;
            unsigned char arcminute;

            file.read((char*)&range,         sizeof(range)        );
            file.read((char*)&degree,        sizeof(degree)       );
            file.read((char*)&arcminute,     sizeof(arcminute)    );

            lTableFile->Range[i]            = range;
            lTableFile->Gradus[i]           = degree;
            lTableFile->Minuta[i]           = arcminute;
        };
        break;
    }
    return lTableFile;
}

/*загрузка файла юстировки*/
JustFile* SupportManager::loadJust(const char *pFileName)
{
    std::fstream file;
    file.open(pFileName, std::ios::in);
    JustFile* lJustFile = new JustFile();
    if (!file) {
        memset(&lJustFile->mJustValues, 0, sizeof(lJustFile->mJustValues));
    } else {
        int len = sizeof(lJustFile->mJustValues) / sizeof(long);
        for (int i = 0; i < len; ++i)
            file >> lJustFile->mJustValues[i];
        file.close();
    }
    return lJustFile;
}

/*сохранение файла юстировки*/
bool SupportManager::saveJust(JustFile *lJustFile, const char *pFileName)
{
    std::fstream file;
    file.open(pFileName, std::ios::out);
    if (!file)
          return false;
    int len = sizeof(lJustFile->mJustValues) / sizeof(long);
    for (int i = 0; i < len; ++i)
        file << lJustFile->mJustValues[i] << std::endl;
    file.close();
    return true;
}
