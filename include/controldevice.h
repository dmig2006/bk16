#ifndef CONTROLDEVICE_H
#define CONTROLDEVICE_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <QTimer>

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <cstdio>
#include <cstdlib>

#include "comport.h"

//define keys
#define KEY_PERMISSION      0x01
#define KEY_AUTO            0x02
#define KEY_SAVE_AIM        0x03
#define KEY_BOLT_NOTCH      0x04
#define KEY_SENSORS         0x05
#define KEY_FIRE_TYPE       0x06
#define KEY_TYPE_BURST      0x07
#define KEY_TRACKING        0x08
#define KEY_AIM_MODE        0x0a
#define KEY_STABILIZATION   0x0b
#define KEY_INVERT          0x13
#define KEY_BLOCKING        0x0c

#ifndef MAKEWORD
#define MAKEWORD(a, b) ((unsigned short)((a & 0xFF) | ((unsigned short)b << 8)))
#endif

struct Key {
    char id;
    int  state;
};

class ControlDevice : public QThread
{
    Q_OBJECT

public:
    ~ControlDevice();
    static ControlDevice* getSingletonPtr();
    void close();
    bool init();
    bool reset();
    bool getADC(unsigned char Number, int &Voltage);
    bool getVoltage(int &Voltage);
    bool getTemperature(int &Termo1, int& Termo0);
    bool getKeyPressed(Key &pKey);
    void clearKeys();
    std::vector<Key> getKeysState();
    bool switchLight(bool enable);

protected:
    void run();

private:
    ControlDevice();
    void updateKeyPressed();
    void updateState();
    bool readKey(Key &pKey);

    ComPort com;
    static ControlDevice *mControlDevice;
    std::list<Key> keys;
    QMutex mMutex;
    QMutex mMutexVoltage;
    bool isClose;
    bool isInit;
    struct timespec time_s;
    struct timespec time_n;
    double h;

signals:
    void newVoltageBu(int voltageExternal);
    void newQueryVoltage();
};

#endif // CONTROLDEVICE_H
