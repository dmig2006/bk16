#include "devicethread.h"
 

/*Конструктор класса*/
DeviceThread::DeviceThread() : is_closed(false)
{
    take_range=0; //Получил значение
    last_range=0;
    tcp_cfg = CfgManager::getSingletonPtr()->getTcpCfg();
    connect(this, SIGNAL(newPermission(bool)),
            ActionList::getSingletonPtr(), SLOT(setPermission(bool)));
}

/*Деструктор класса*/
DeviceThread::~DeviceThread()
{
    close();
    wait(2000);
}

/*установка флага на закрытие*/
void DeviceThread::close()
{
    mutex.lock();
    is_closed = true;
    mutex.unlock();
}

/*соединение и взаимодействие с ПП*/
void DeviceThread::run()
{

    emit newPermission(false);
    while (!is_closed) {
        try {
            TCPClient tcp_client(tcp_cfg->maddr, tcp_cfg->mPort);

            emit newPermission(true);
            emit newLogMsg(tr("Device drive connected"));
            emit newConnectedDevice(true);

            while (!is_closed)
                update(tcp_client);

        } catch (TCPConnectionError) {
            usleep(500000);
            continue;
        } catch (TCPError) {
            /* SOME OTHER TCP EXCEPTION */
        } catch (...) {
            /* SOME OTHER UNEXPECTED EXCEPTION */
        }

        emit newPermission(false);
        emit newLogMsg(tr("Device drive not connected"));
        emit newConnectedDevice(false);
    }
}

/*чтение/запись команд*/
void DeviceThread::update(TCPClient &tcp_client)
{
    Action *action = ActionList::getSingletonPtr()->getAction();
    if (action) {
        tcp_client.writeAction(action);
	parsingResponse(tcp_client.readAction());
    } else {
        msleep(20);
    }
}

/*обработка ответов от ПП на команды*/
void DeviceThread::parsingResponse(Action *action)
{
    switch (action->getActionType()) {
    case ACTION_TYPE_ENCODER_SPEED_X:
        emit newSpeedEncoderX(action->getValue());
        break;
    case ACTION_TYPE_ENCODER_SPEED_Y:
        emit newSpeedEncoderY(action->getValue());
        break;
    case ACTION_TYPE_ENCODER_POSITION_X:
        emit newPositionEncoderX(action->getValue());
        break;
    case ACTION_TYPE_ENCODER_POSITION_Y:
        emit newPositionEncoderY(action->getValue());
        break;
    case ACTION_TYPE_TRMO_0:
        emit newThermo(action->getValue());
        break;
    case ACTION_TYPE_PRESSURE:
        emit newPressure(action->getValue(), true);
        break;
    case ACTION_TYPE_AMMUNITION_COUNTER:
        emit newAmmunitionCounter(action->getValue(), false);
        break;
    case ACTION_TYPE_RANGE:
    {
     #ifdef AVATAR_SDELAL
        take_range=action->getValue();

            if(take_range>0) last_range=take_range;
            else emit newLogMsg("LD Range Error "+QString::number(take_range));
            if(last_range<100) last_range=100;
            emit newRange(last_range / 100);
     #else
        emit newRange(action->getValue() / 100);
     #endif

        break;
    }
    case ACTION_TYPE_ACHIEVEMENT_POSITION:
        emit newAchievementPosition(action->getValue());
        break;
    case ACTION_TYPE_VOLTAGE_EXTERNAL:
        emit newVoltageExternal(action->getValue());
        break;
    case ACTION_TYPE_VOLTAGE_BATTERY:
        emit newVoltageBattery(action->getValue());
        break;
    case ACTION_TYPE_CLOSE:
        emit newClose();
        break;
    case ACTION_TYPE_STAND_BY_MODE:
    case ACTION_TYPE_OPERATION_MODE :
        break;
    case ACTION_TYPE_BOLT_NOTCH :
        emit newResBoltNotch(action->getValue());
        break;
    }
    delete action;
}
