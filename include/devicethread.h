#ifndef DEVICETHREAD_H
#define DEVICETHREAD_H
 
#include <QThread>
#include <QMutex>

#include "supportmanager.h"
#include "tcpclient.h"
#include "cfgmanager.h"
#include "actionlist.h"
#include "action.h"

class DeviceThread : public QThread
{
    Q_OBJECT

    QMutex mutex;
    bool is_closed;
    TcpCfg *tcp_cfg;

    void update(TCPClient &client);
    void parsingResponse(Action *action);

protected:
    void run();

public:
    DeviceThread();
    ~DeviceThread();
    void close();

#ifdef AVATAR_SDELAL
    int take_range; //Получил значение
    int last_range; //последнее правильное значение
#endif

signals:
    void newConnectedDevice(bool state);
    void newLogMsg(QString string);
    void newSpeedEncoderX(long speed);
    void newSpeedEncoderY(long speed);
    void newPositionEncoderX(long position);
    void newPositionEncoderY(long position);
    void newThermo(long thermo);
    void newPressure(long pressure, bool);
    void newAmmunitionCounter(long counter, bool state);
    void newRange(long range);
    void newAchievementPosition(bool state);
    void newVoltageExternal(long voltage);
    void newVoltageBattery(long voltage);
    void newClose();
    void newResBoltNotch(int res);
    void newPermission(bool allow);
};

#endif // DEVICETHREAD_H
