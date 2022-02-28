#include "controldevice.h"

ControlDevice* ControlDevice::mControlDevice = 0;

/*Конструктор класса*/
ControlDevice::ControlDevice()
{
    isClose = false;
    isInit = init();
    clock_gettime(CLOCK_MONOTONIC, &time_s);
}

/*Деструктор класса*/
ControlDevice::~ControlDevice()
{
    close();
    wait(2000);
}

/*создание только одного экземпляра класса с глобальной точкой доступа*/
ControlDevice* ControlDevice::getSingletonPtr()
{
    if (!mControlDevice)
        mControlDevice = new ControlDevice();
    return mControlDevice;
}

/*закрытие соединения*/
void ControlDevice::close()
{
    isClose = true;
    com.close();
    isInit = false;
}

/*открытие соединения*/
bool ControlDevice::init()
{
    char port[] = "/dev/ttyS1";
    return com.open(port);
}

/*сброс БК*/
bool ControlDevice::reset()
{
    unsigned char writeBuffer[5] = {0};
    unsigned char readBuffer[4] = {0};
    writeBuffer[0] = 0x55;
    writeBuffer[1] = 0x00;
    writeBuffer[2] = 0x00;
    writeBuffer[3] = 0x00;
    writeBuffer[4] = 0x55;
    if (!com.write(writeBuffer, 5))
        return false;
    if (!com.read(readBuffer, 4))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 'R'
            && readBuffer[2] == 'S' && readBuffer[3] == 0xFF)
        return true;
    return false;
}

/*чтение АЦП*/
bool ControlDevice::getADC(unsigned char Number, int &Voltage)
{
    unsigned char writeBuffer[5] = {0};
    unsigned char readBuffer[5] = {0};
    writeBuffer[0] = 0x55;
    writeBuffer[1] = 0x02;
    writeBuffer[2] = 0x00;
    writeBuffer[3] = Number;
    writeBuffer[4] = crc8(writeBuffer, 4);
    if (!com.write(writeBuffer, 5))
        return false;
    if (!com.read(readBuffer, 5))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 0x02
            && readBuffer[4] == crc8(readBuffer, 4)) {
        Voltage = (int)(readBuffer[2]) << 8;
        Voltage |= (int)(readBuffer[3]);
        return true;
    }
    return false;
}

/*получение напряжения питания*/
bool ControlDevice::getVoltage(int &Voltage)
{
    unsigned char writeBuffer[5] = {0};
    unsigned char readBuffer[5] = {0};
    writeBuffer[0] = 0x55;
    writeBuffer[1] = 0x07;
    writeBuffer[2] = 0x00;
    writeBuffer[3] = 0x00;
    writeBuffer[4] = crc8(writeBuffer, 4);
    if (!com.write(writeBuffer, 5))
        return false;
    if (!com.read(readBuffer, 5))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 0x07
            && readBuffer[4] == crc8(readBuffer, 4)) {
        Voltage = (int)(readBuffer[2]) << 8;
        Voltage |= (int)(readBuffer[3]);
        return true;
    }
    return false;
}

/*получение температуры*/
bool ControlDevice::getTemperature(int &Termo1, int& Termo0)
{
    unsigned char writeBuffer[5] = {0};
    unsigned char readBuffer[5] = {0};
    writeBuffer[0] = 0x55;
    writeBuffer[1] = 0x03;
    writeBuffer[2] = 0x00;
    writeBuffer[3] = 0x00;
    writeBuffer[4] = crc8(writeBuffer, 4);
    if (!com.write(writeBuffer, 5))
        return false;
    if (!com.read(readBuffer, 5))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 0x03
            && readBuffer[4] == crc8(readBuffer, 4)) {
        Termo1 = readBuffer[2];
        Termo0 = readBuffer[3];
        return true;
    }
    return false;
}

/*получение кнопки из списка*/
bool ControlDevice::getKeyPressed(Key &pKey)
{
    bool isKey = false;
    mMutex.lock();
    if (keys.size() > 0) {
        pKey = keys.back();
        keys.pop_back();
        isKey = true;
    }
    mMutex.unlock();
    return isKey;
}

/*очистка списка кнопок*/
void ControlDevice::clearKeys()
{
    Key lKey={0,0};
    int state_perm=0, state_block=0;
    bool presed_perm=false, presed_block=false;

    mMutex.lock();
    while(keys.size() > 0){
        lKey = keys.back();
        keys.pop_back();
        if(lKey.id==KEY_PERMISSION){
            state_perm=lKey.state;
            presed_perm=true;
        }
        if(lKey.id==KEY_BLOCKING){
            state_block=lKey.state;
            presed_block=true;
        }
       }
   
    if(presed_perm){
        lKey.id=KEY_PERMISSION;
        lKey.state=state_perm;
        if (keys.size() < 20)
            keys.push_front(lKey);
    }
    if(presed_block){
        lKey.id=KEY_BLOCKING;
        lKey.state=state_block;
        if (keys.size() < 20)
            keys.push_front(lKey);
    }
    mMutex.unlock();
}

/*чтение состояния кнопок*/
std::vector<Key> ControlDevice::getKeysState()
{
    std::vector<Key> keys;

    Key lKeyPermission;
    lKeyPermission.id = KEY_PERMISSION;
    lKeyPermission.state = 0;

    Key lKeyAuto;
    lKeyAuto.id = KEY_AUTO;
    lKeyAuto.state = 0;

    Key lKeySaveAim;
    lKeySaveAim.id = KEY_SAVE_AIM;
    lKeySaveAim.state = 0;

    Key lKeyBoltNotch;
    lKeyBoltNotch.id = KEY_BOLT_NOTCH;
    lKeyBoltNotch.state = 0;

    Key lKeySensors;
    lKeySensors.id = KEY_SENSORS;
    lKeySensors.state = 0;

    Key lKeyFireType;
    lKeyFireType.id = KEY_FIRE_TYPE;
    lKeyFireType.state = 0;

    Key lKeyTypeBrush;
    lKeyTypeBrush.id = KEY_TYPE_BURST;
    lKeyTypeBrush.state = 0;

    Key lKeyTracking;
    lKeyTracking.id = KEY_TRACKING;
    lKeyTracking.state = 0;

    Key lKeyBlocking;
    lKeyBlocking.id = KEY_BLOCKING;
    lKeyBlocking.state = 0;

    Key lKeyAimMode;
    lKeyAimMode.id = KEY_AIM_MODE;
    lKeyAimMode.state = 0;

    Key lKeyStabilization;
    lKeyStabilization.id = KEY_STABILIZATION;
    lKeyStabilization.state = 0;

    Key lKeyInvert;
    lKeyInvert.id = KEY_INVERT;
    lKeyInvert.state = 0;

    keys.push_back(lKeyPermission);
    keys.push_back(lKeyAuto);
    keys.push_back(lKeySaveAim);
    keys.push_back(lKeyBoltNotch);
    keys.push_back(lKeySensors);
    keys.push_back(lKeyFireType);
    keys.push_back(lKeyTypeBrush);
    keys.push_back(lKeyTracking);
    keys.push_back(lKeyAimMode);
    keys.push_back(lKeyStabilization);
    keys.push_back(lKeyInvert);
    keys.push_back(lKeyBlocking);

    for (unsigned int i = 0; i < keys.size(); ++i) {
        unsigned char writeBuffer[5] = {0};
        unsigned char readBuffer[5] = {0};
        writeBuffer[0] = 0x55;
        writeBuffer[1] = 0x01;
        writeBuffer[2] = 0x00;
        writeBuffer[3] = keys[i].id;
        writeBuffer[4] = crc8(writeBuffer, 4);
        if (com.write(writeBuffer, 5) && com.read(readBuffer, 5)
                && readBuffer[0] == 0xaa && readBuffer[1] == 0x01
                && readBuffer[4] == crc8(readBuffer, 4)) {
            keys[i].id = readBuffer[3];
            keys[i].state = readBuffer[2];
        }
    }
    return keys;
}

/*подсветка экрана*/
bool ControlDevice::switchLight(bool enable)
{
    unsigned char writeBuffer[5] = {0};
    unsigned char readBuffer[5] = {0};
    writeBuffer[0] = 0x55;
    writeBuffer[1] = 0x0A;
    writeBuffer[2] = 0x00;
    if (enable)
        writeBuffer[3] = 0x01;
    else
        writeBuffer[3] = 0x00;
    writeBuffer[4] = crc8(writeBuffer, 4);
    if (!com.write(writeBuffer, 5))
        return false;
    if (!com.read(readBuffer, 5))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 0xA && readBuffer[2] == 0x00) {
        if (enable && readBuffer[3] == 0x01 && readBuffer[4] == crc8(readBuffer, 4))
            return true;
        else if (readBuffer[3] == 0x00 && readBuffer[4] == crc8(readBuffer, 4))
            return true;
    }
    return true;
}

/*цикл опроса БК*/
void ControlDevice::run()
{
    if (isInit) {
        while (!isClose) {
            usleep(150000);
            updateKeyPressed();
            clock_gettime(CLOCK_MONOTONIC, &time_n);
            double time = static_cast<double>(time_n.tv_sec - time_s.tv_sec)
                          + static_cast<double>(time_n.tv_nsec - time_s.tv_nsec)
                            / 1000000000.0;
            if (time > 5.0) {
                clock_gettime(CLOCK_MONOTONIC, &time_s);
                updateState();
            }
        }
    }
}

/*добавление нажатой кнопки в список*/
void ControlDevice::updateKeyPressed()
{
    Key lKey;
    bool state = readKey(lKey);
    if (state) {
        mMutex.lock();
        if (keys.size() < 20)
            keys.push_front(lKey);
        mMutex.unlock();
    }
}

/*оновление напряжения питания*/
void ControlDevice::updateState()
{
    int lVoltageExternal = 0;
    getVoltage(lVoltageExternal);
    emit newVoltageBu(lVoltageExternal);
    //query voltag
    emit newQueryVoltage();
}

/*чтение нажатой кнопки*/
bool ControlDevice::readKey(Key &pKey)
{
    unsigned char readBuffer[5] = {0};
    if (!com.read(readBuffer, 5))
        return false;
    if (readBuffer[0] == 0xAA && readBuffer[1] == 0x01) {
        pKey.id = readBuffer[3];
        pKey.state = readBuffer[2];
        return true;
    }
    return false;
}
