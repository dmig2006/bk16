#include "controllermanager.h"
  
/*Конструктор класса*/
ControllerManager::ControllerManager()
{
    mDeviceThread        = new DeviceThread();
    mJoystick            = new Joystick();
    mControlDevice       = ControlDevice::getSingletonPtr();

    isClose              = false;
    mCurrentSpeedX       = 0;
    mCurrentSpeedY       = 0;

    //Joystick--------*-------------------
    lPosJoyX             = 0;
    lPosJoyY             = 0;
    enableJoystick       = true;
    pidX		 = 0;
    pidY		 = 0;
    VsrXX = 0;
    VsrYY = 0;
    fKey                 = 1;

    spVedX = 0;
    spVedY = 0;
    //Joystick--------------------------

    mCurrentPositionX    = 0;
    mCurrentPositionY    = 0;

    mTrackingCenterX = 0;
    mTrackingCenterY = 0;

    mTrackingCenterX_rec = 0;
    mTrackingCenterY_rec = 0;

    mCorrectionTrackX    = 0.f;
    mCorrectionTrackY    = 0.f;

    mTrackingX           = 0;
    mTrackingY           = 0;

    mCorrectionX         = 0;
    mCorrectionY         = 0;
    isPositionReached    = false;
    isCorrectionPosition = true;

    mPositionEncoderX    = 0;
    mPositionEncoderY    = 0;

    mTracking_res_x      = 0;
    mTracking_res_y      = 0;

    int_part_x           = 0.0;
    diff_part_x          = 0.0;
    int_part_y           = 0.0;
    diff_part_y          = 0.0;

    mAmmunitionCounter  = 0;

    mBurstShort          = 2;
    mBurstLong           = 6;
    mBurstType           = SHORT_BURST;
    mLastFireType        = SINGLE_FIRE;
    mFireType            = SINGLE_FIRE;
    mWeaponType          = WEAPON_KORD;
    mMode                = MODE_SEMI_AUTO;
    mAutoType            = AUTO_TYPE_ORDER;
    mWindX               = 0;
    mWindY               = 0;

    //Zala AS
    enabled_as           = true;
    //Zala AS

    mPermission          = false;
    mBlocking            = false;
    mAddFireProcess      = false;
    isFire               = false;
    isConnectedDevice    = false;
    isStop               = false;
    setPermission(mPermission);

    mRange               = 100;
    isNewRange           = false;

    mStabilization       = false;
    mTracking            = false;
    mTypeTracking        = 0;

    mVoltageExternalBu = 0;
    mVoltageBatteryBu  = 0;

    mIndexCurrentAim = 0;

    isResetFire       = true;
    isFirstAimByCoord = false;

    mKeyState = new JoyKeyState();
    mKeyState->keyCameraNarrow = false;
    mKeyState->keyCameraThermal = false;
    mKeyState->keyCameraWide = false;
    mKeyState->keyFire = false;
    mKeyState->keyRange = false;

    mJustCfg[ 0] = CfgManager::getSingletonPtr()->getJust(1, WEAPON_KORD);
    mJustCfg[ 1] = CfgManager::getSingletonPtr()->getJust(2, WEAPON_KORD);
    mJustCfg[ 2] = CfgManager::getSingletonPtr()->getJust(3, WEAPON_KORD);
    mJustCfg[ 3] = CfgManager::getSingletonPtr()->getJust(4, WEAPON_KORD);

    mJustCfg[ 4] = CfgManager::getSingletonPtr()->getJust(1, WEAPON_PKT);
    mJustCfg[ 5] = CfgManager::getSingletonPtr()->getJust(2, WEAPON_PKT);
    mJustCfg[ 6] = CfgManager::getSingletonPtr()->getJust(3, WEAPON_PKT);
    mJustCfg[ 7] = CfgManager::getSingletonPtr()->getJust(4, WEAPON_PKT);

    mJustCfg[ 8] = CfgManager::getSingletonPtr()->getJust(1, WEAPON_AGS);
    mJustCfg[ 9] = CfgManager::getSingletonPtr()->getJust(2, WEAPON_AGS);
    mJustCfg[10] = CfgManager::getSingletonPtr()->getJust(3, WEAPON_AGS);
    mJustCfg[11] = CfgManager::getSingletonPtr()->getJust(4, WEAPON_AGS);

    mJustCfg[12] = CfgManager::getSingletonPtr()->getJust(1, WEAPON_AG40);
    mJustCfg[13] = CfgManager::getSingletonPtr()->getJust(2, WEAPON_AG40);
    mJustCfg[14] = CfgManager::getSingletonPtr()->getJust(3, WEAPON_AG40);
    mJustCfg[15] = CfgManager::getSingletonPtr()->getJust(4, WEAPON_AG40);

    mAmendmentX = 0;
    mAmendmentY = 0;

    mThermo = 15;
    mPressure = 750;

    mCameraMode = WIDE_FORMAT;

    mAimType = HOME;

    //init timers
    mTimerEncoder = new QTimer(this);
    mTimerKey = new QTimer(this);
    connect(mTimerEncoder, SIGNAL(timeout()), SLOT(queryEncoders()));
    connect(mTimerKey, SIGNAL(timeout()), SLOT(updateKeys()));

    connect(mJoystick,     SIGNAL(newLogMsg(QString)),              SIGNAL(newLogMsg(QString)));
    connect(mDeviceThread, SIGNAL(newLogMsg(QString)),              SIGNAL(newLogMsg(QString)));
    connect(mDeviceThread, SIGNAL(newConnectedDevice(bool)),        SLOT(setConnectedDevice(bool)));
    connect(mDeviceThread, SIGNAL(newSpeedEncoderX(long)),          SLOT(setSpeedEncoderX(long)));
    connect(mDeviceThread, SIGNAL(newSpeedEncoderY(long)),          SLOT(setSpeedEncoderY(long)));
    connect(mDeviceThread, SIGNAL(newPositionEncoderX(long)),       SLOT(setPositionEncoderX(long)));
    connect(mDeviceThread, SIGNAL(newPositionEncoderY(long)),       SLOT(setPositionEncoderY(long)));
    connect(mDeviceThread, SIGNAL(newThermo(long)),                 SLOT(setThermo(long)));
    connect(mDeviceThread, SIGNAL(newPressure(long, bool)),         SLOT(setPressure(long, bool)));
    connect(mDeviceThread, SIGNAL(newAmmunitionCounter(long,bool)), SLOT(setAmmunitionCounter(long,bool)));
    connect(mDeviceThread, SIGNAL(newRange(long)),                  SLOT(setRange(long)));
    connect(mDeviceThread, SIGNAL(newAchievementPosition(bool)),    SLOT(setAchievementPosition(bool)));
    connect(mDeviceThread, SIGNAL(newVoltageExternal(long)),        SLOT(setVoltageExternal(long)));
    connect(mDeviceThread, SIGNAL(newVoltageBattery(long)),         SLOT(setVoltageExternal(long)));
    connect(mDeviceThread, SIGNAL(newClose()),                      SLOT(close()));
    connect(mDeviceThread, SIGNAL(newResBoltNotch(int)),            SLOT(setResBoltNotch(int)));

    connect(mControlDevice, SIGNAL(newQueryVoltage()), SLOT(queryVoltage()));
    connect(mControlDevice, SIGNAL(newVoltageBu(int)), SLOT(setVoltageBu(int)));

    //get keys state
    std::vector<Key> keys = mControlDevice->getKeysState();
    for (unsigned int i = 0; i < keys.size(); ++i) {
        if (keys[i].id == KEY_PERMISSION) {
            if (keys[i].state == 0)
                setPermission(false);
            else
                setPermission(true);
        }
    }
    flagEsc = false;
    mTimerEncoder->start(250);
    mTimerKey->start(100);
    mDeviceThread->start();
    mControlDevice->start();
    mJoystick->open();
    is_load_enabled = true;
    shotCounter = 0;
    need_exit = false;
    lightEnabled = true;
#ifdef AVATAR_SDELAL
    count_range=0;
    state_range=0;
    timer_range=0;
#endif

    //упреждение
    init_tabl_upr();
    f_uprGor = true;
    f_uprVer = false;
    //упреждение

    readShotTimes();
}

/*Деструктор класса*/
ControllerManager::~ControllerManager()
{
    stopExecution();
    msleep(200);
    writeShotTimes();
    delete mTimerEncoder;
    delete mKeyState;
    delete mControlDevice;
    delete mJoystick;
    delete mDeviceThread;
    delete mTimerKey;
    wait(1000);
}

/*установка в ноль и выключение ПП*/
void ControllerManager::closeDevice()
{
    //ACTION_TO_ZERO
    mMutexConnectedDevice.lock();
    if (!isConnectedDevice) {
        mMutexConnectedDevice.unlock();
        close();
    } else {
        mMutexConnectedDevice.unlock();
        emit newLogMsg(tr("Platform Shutdown"));
        Action *close_action = new Action(ACTION_TYPE_CLOSE, 0);
        ActionList::getSingletonPtr()->setAction(close_action);
        msleep(1000);
    }
}

/*установка флага на блокировку функциональной клавиатуры и джойстика*/
void ControllerManager::setPause(bool state)
{
    mMutexStoped.lock();
    isStop = state;
    setSpeedDeviceDrive(0,0);
    mMutexStoped.unlock();
}

/*установка режима работы*/
void ControllerManager::setMode(Mode pMode)
{
    mMutexMode.lock();
    mMode = pMode;
    mMutexMode.unlock();
    mMutexTracking.lock();
    mTrackingCenterX = 0;
    mTrackingCenterY = 0;
    mMutexTracking.unlock();
}

/* перевод ПП в дежурный режим*/
void ControllerManager::setStandbyMode()
{
    msleep(300);
    Action *action = new Action(ACTION_TYPE_STAND_BY_MODE, 0);
    ActionList::getSingletonPtr()->setAction(action);
}

/*перевод ПП в рабочий режим*/
void ControllerManager::setOperationMode()
{
    Action *action = new Action(ACTION_TYPE_OPERATION_MODE, 0);
    ActionList::getSingletonPtr()->setAction(action);
}

/*остановка движения ПП*/
void ControllerManager::setStopDeviceDrive()
{
    Action *lActionStopXY = new Action(ACTION_TO_MODE_STOP, 0);
    ActionList::getSingletonPtr()->setAction(lActionStopXY);
}

/*установка в нулевое положение ПП*/
void ControllerManager:: setMoveToZeroDeviceDrive()
{
    Action *lActionBrake = new Action(ACTION_TYPE_SET_BRAKE, false);
    ActionList::getSingletonPtr()->setAction(lActionBrake);
    Action *lActionMoveToZero = new Action(ACTION_TO_ZERO, 0);
    ActionList::getSingletonPtr()->setAction(lActionMoveToZero);
        isPositionReached = false;
    msleep(50);
    while (!isPositionReached) {
        queryAchievementPosition();
        if(mBlocking){
            setStopDeviceDrive();
            break;
        }
        msleep(100);
    }
    Action *lActionCoordMode = new Action(ACTION_TO_MODE_COORD_CONTROL, 0);
    ActionList::getSingletonPtr()->setAction(lActionCoordMode);
}

/*установка ПП в соответсвующее пололожение (относительное позиционирование)*/
void ControllerManager::setPositionDeviceDrive(long pPositionX, long pPositionY,
                                               bool waitAchievementPosition)
{
    Action *lActionPositionX = new Action(ACTION_TYPE_POSITION_X, pPositionX);
    Action *lActionPositionY = new Action(ACTION_TYPE_POSITION_Y, pPositionY);
    if (((!enabled_as) && (!lPosJoyX)) && (!lPosJoyY)) emit acRamkaFreez(true);
    ActionList::getSingletonPtr()->setAction(lActionPositionX);
    ActionList::getSingletonPtr()->setAction(lActionPositionY);
    emit newReset();
    //is wait achievement
    if (waitAchievementPosition) 
    {
        isPositionReached = false;
        msleep(50);
	while (!isPositionReached) {
            queryAchievementPosition();
            if(mBlocking)
            {
                setStopDeviceDrive();
                break;
            }
            msleep(100);
        }
    }
   
}

/*установка скорости перемещения ПП*/
void ControllerManager::setSpeedDeviceDrive(long pSpeedX, long pSpeedY)
{
        if (pSpeedX != mCurrentSpeedX || pSpeedY != mCurrentSpeedY) {
        Action *action_speed_x;
        Action *action_speed_y;
        action_speed_x = new Action(ACTION_TYPE_SPEED_X, pSpeedX);
        action_speed_y = new Action(ACTION_TYPE_SPEED_Y, pSpeedY);
        mCurrentSpeedX = pSpeedX;
        mCurrentSpeedY = pSpeedY;
        ActionList::getSingletonPtr()->setAction(action_speed_x);
        ActionList::getSingletonPtr()->setAction(action_speed_y);
        emit newReset();
    }
}

/*установка состояния режима сопровождения*/
void ControllerManager::setTracking(bool enabled)
{
    mTracking = enabled;
}

/*включение/выключение цифрового увеличения тепловизора*/
void ControllerManager::setZoomTermo(int pState)
{
    Action *action = new Action(ACTION_TYPE_ZOOM_THERMO, pState);
    ActionList::getSingletonPtr()->setAction(action);
}

/*установка типа длинны очереди*/
void ControllerManager::setBurstType(BurstType burst_type)
{
    mBurstType = burst_type;
    emit newBurstType(mBurstType);
}

/*установка координат основного перекрестия*/
void ControllerManager::setJust(long pJustX, long pJustY)
{
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexJust.lock();
    switch (lCameraMode) {
    case WIDE_FORMAT:
        mJustCfg[mWeaponType * 4 + 0].JustX = pJustX;
        mJustCfg[mWeaponType * 4 + 0].JustY = pJustY;
        break;
    case NARROW_FORMAT:
        mJustCfg[mWeaponType * 4 + 1].JustX = pJustX;
        mJustCfg[mWeaponType * 4 + 1].JustY = pJustY;
        break;
    case THERMAL:
        mJustCfg[mWeaponType * 4 + 2].JustX = pJustX;
        mJustCfg[mWeaponType * 4 + 2].JustY = pJustY;
        break;
    case ZOOM_THERMAL:
        mJustCfg[mWeaponType * 4 + 3].JustX = pJustX;
        mJustCfg[mWeaponType * 4 + 3].JustY = pJustY;
        break;
    }
    mMutexJust.unlock();
    emit newCorrectionCross(0, 0);
    emit newJust(-pJustX, pJustY);
}

/*установка корректировки точки ведения цели в режиме сопровождения*/
void ControllerManager::setCorrectionTracking(int x, int y)
{
    mMutexTracking.lock();
    mCorrectionTrackX += x;
    mCorrectionTrackY += y;
    mMutexTracking.unlock();
}

/*установка типа стрельбы*/
void ControllerManager::setFireType(FireType fire_type)
{
    mFireType = fire_type;
    emit newFireType(mFireType);
}

/*расчет и установка скорости для атомата сопровождения*/
void ControllerManager::setCenterMassTracking(int x, int y)
{
    long mTracking_res_x_int;
    long mTracking_res_y_int;
    double k_p = 1.25;
    double k_int = 65.0;
    mMutexTracking.lock();
    long lX = x - DEFAULT_WIDTH / 2;
    long lY = y - DEFAULT_HEIGHT / 2;
    mMutexTracking.unlock();
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexTracking.lock();

    switch (lCameraMode) {
    case NARROW_FORMAT:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_NARROW_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_NARROW_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case WIDE_FORMAT:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_WIDE_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_WIDE_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case THERMAL:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case ZOOM_THERMAL:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_ZOOM_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_ZOOM_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    }

    mTracking_res_x = mTrackingCenterX;
    mTracking_res_y = mTrackingCenterY;

    lX += (int)mCorrectionTrackX;
    lY += (int)mCorrectionTrackY;

    switch (lCameraMode) {
    case NARROW_FORMAT:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_NARROW_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_NARROW_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case WIDE_FORMAT:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_WIDE_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_WIDE_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case THERMAL:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case ZOOM_THERMAL:
        mTrackingCenterX = static_cast<long>(static_cast<double>(-lX)
                                             * ANGLE_ZOOM_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(static_cast<double>(-lY)
                                             * ANGLE_ZOOM_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    }

    mTracking_res_x_int = mTrackingCenterX;
    mTracking_res_y_int = mTrackingCenterY;

    switch (lCameraMode) {
    case NARROW_FORMAT:
        mTrackingCenterX = static_cast<long>(-k_p * static_cast<double>(lX)
                                             * ANGLE_NARROW_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(-k_p * static_cast<double>(lY)
                                             * ANGLE_NARROW_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case WIDE_FORMAT:
        mTrackingCenterX = static_cast<long>(-k_p * static_cast<double>(lX)
                                             * ANGLE_WIDE_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(-k_p * static_cast<double>(lY)
                                             * ANGLE_WIDE_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case THERMAL:
        mTrackingCenterX = static_cast<long>(-k_p * static_cast<double>(lX)
                                             * ANGLE_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(-k_p * static_cast<double>(lY)
                                             * ANGLE_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    case ZOOM_THERMAL:
        mTrackingCenterX = static_cast<long>(-k_p * static_cast<double>(lX)
                                             * ANGLE_ZOOM_THERMAL_X
                                             / static_cast<double>(DEFAULT_WIDTH)
                                             * DEGREE_TO_CENTIDEGREE);
        mTrackingCenterY = static_cast<long>(-k_p * static_cast<double>(lY)
                                             * ANGLE_ZOOM_THERMAL_Y
                                             / static_cast<double>(DEFAULT_HEIGHT)
                                             * DEGREE_TO_CENTIDEGREE);
        break;
    }

    int_part_x += mTracking_res_x_int / k_int;
    int_part_y += mTracking_res_y_int / k_int;
    mTrackingCenterX += int_part_x;
    mTrackingCenterY += int_part_y;

    switch (lCameraMode) {
    case NARROW_FORMAT:
        if (mTrackingCenterX > MAX_SPEED_NARROW_FORMAT_X)
            mTrackingCenterX = MAX_SPEED_NARROW_FORMAT_X;
        if (mTrackingCenterX < -MAX_SPEED_NARROW_FORMAT_X)
            mTrackingCenterX = -MAX_SPEED_NARROW_FORMAT_X;
        if (mTrackingCenterY > MAX_SPEED_NARROW_FORMAT_Y)
            mTrackingCenterY = MAX_SPEED_NARROW_FORMAT_Y;
        if (mTrackingCenterY < -MAX_SPEED_NARROW_FORMAT_Y)
            mTrackingCenterY = -MAX_SPEED_NARROW_FORMAT_Y;
        break;
    case WIDE_FORMAT:
        if (mTrackingCenterX > MAX_SPEED_WIDE_FORMAT_X)
            mTrackingCenterX = MAX_SPEED_WIDE_FORMAT_X;
        if (mTrackingCenterX < -MAX_SPEED_WIDE_FORMAT_X)
            mTrackingCenterX = -MAX_SPEED_WIDE_FORMAT_X;
        if (mTrackingCenterY > MAX_SPEED_WIDE_FORMAT_Y)
            mTrackingCenterY = MAX_SPEED_WIDE_FORMAT_Y;
        if (mTrackingCenterY < -MAX_SPEED_WIDE_FORMAT_Y)
            mTrackingCenterY = -MAX_SPEED_WIDE_FORMAT_Y;
        break;
    case THERMAL:
        if (mTrackingCenterX > MAX_SPEED_THERMAL_X)
            mTrackingCenterX = MAX_SPEED_THERMAL_X;
        if (mTrackingCenterX < -MAX_SPEED_THERMAL_X)
            mTrackingCenterX = -MAX_SPEED_THERMAL_X;
        if (mTrackingCenterY > MAX_SPEED_THERMAL_Y)
            mTrackingCenterY = MAX_SPEED_THERMAL_Y;
        if (mTrackingCenterY < -MAX_SPEED_THERMAL_Y)
            mTrackingCenterY = -MAX_SPEED_THERMAL_Y;
        break;
    case ZOOM_THERMAL:
        if (mTrackingCenterX > MAX_SPEED_ZOOM_THERMAL_X)
            mTrackingCenterX = MAX_SPEED_ZOOM_THERMAL_X;
        if (mTrackingCenterX < -MAX_SPEED_ZOOM_THERMAL_X)
            mTrackingCenterX = -MAX_SPEED_ZOOM_THERMAL_X;
        if (mTrackingCenterY > MAX_SPEED_ZOOM_THERMAL_X)
            mTrackingCenterY = MAX_SPEED_ZOOM_THERMAL_X;
        if (mTrackingCenterY < -MAX_SPEED_ZOOM_THERMAL_X)
            mTrackingCenterY = -MAX_SPEED_ZOOM_THERMAL_X;
        break;
    }


    if (abs(mTrackingCenterX) > abs(mTrackingCenterX_rec))
        mTrackingCenterX = (mTrackingCenterX - mTrackingCenterX_rec) / 2;
    if (abs(mTrackingCenterY) > abs(mTrackingCenterY_rec))
        mTrackingCenterY = (mTrackingCenterY + mTrackingCenterY_rec) / 2;

    mTrackingCenterX = static_cast<long>(static_cast<double>(mTrackingCenterX)
                                         * 425.0 / DEGREE_TO_CENTIDEGREE / 6.0);
    mTrackingCenterY = static_cast<long>(static_cast<double>(mTrackingCenterY)
                                         * 512.5 / DEGREE_TO_CENTIDEGREE / 6.0);

    mMutexTracking.unlock();
}

/*установка скорости ветра бокового*/
void ControllerManager::setWindX(long pWindX)
{
    mWindX = pWindX;
    correctionToFire();
}

/*установка скорости ветра встресного*/
void ControllerManager::setWindY(long pWindY)
{
    mWindY = pWindY;
    correctionToFire();
}

/*установка типа вооружения*/
void ControllerManager::setWeapon(WeaponType pWeaponType)
{
    mWeaponType = pWeaponType;
    mTrackingAmendment.setWeaponType(pWeaponType);
}

/*установка длины короткой очереди*/
void ControllerManager::setShortBurst(long pBurstShort)
{
    mBurstShort = pBurstShort;
}

/*установка длины длинной очереди*/
void ControllerManager::setLongBurst(long pBurstLong)
{
    mBurstLong = pBurstLong;
}

/*установка типа сопровождения*/
void ControllerManager::setTypeTracking(int pType)
{
    mMutexTracking.lock();
    mTypeTracking = pType;
    mMutexTracking.unlock();
}

/*установка давления окружающей среды*/
///разрешение огня
void ControllerManager::setPermission(bool state)
{
    mMutexPermission.lock();
    mPermission = state;
    mMutexPermission.unlock();
    emit newPermission(state);
}

/*установка типа атоматического режима*/
void ControllerManager::setAutoType(AutoType pAutoType)
{
    mAutoType = pAutoType;
}

/*установка последней цели после выхода из редактора целей*/
void ControllerManager::setDisplayLastAim(Aim *lAim)
{
    if (lAim)
        emit newLastSaved(lAim->mPositionX, lAim->mPositionY,
                          lAim->mRange, lAim->mTarget);
}

/*установка флага нажатия на Esc*/
void ControllerManager::setFlagEsc()
{
    flagEsc = true;
}

/*переключение зума тепловизора при соответствующем типе видеокамеры*/
void ControllerManager::setCamera(CameraMode pCameraMode)
{
    mMutexCamera.lock();
    mMutexMode.lock();
    if (mMode != MODE_TRACKING) {
        mCameraMode = pCameraMode;
        switch (mCameraMode) {
        case NARROW_FORMAT:
        case WIDE_FORMAT:
            break;
        case THERMAL:
            setZoomTermo(0);
            break;
        case ZOOM_THERMAL:
            setZoomTermo(1);
            break;
        }
        emit newCameraMode(pCameraMode);
    }
    mMutexMode.unlock();
    mMutexCamera.unlock();
}

/*установка длительности одного выстрела*/
void ControllerManager::setShotTime(int s_time)
{
    if (shot_times.find(mWeaponType) != shot_times.end()) {
        if (shot_times[mWeaponType] != s_time) {
            shot_times[mWeaponType] = s_time;
            is_shot_times_changed = true;
        }
    } else {
        shot_times.insert(std::pair<WeaponType, int>(mWeaponType, s_time));
        is_shot_times_changed = true;
    }
}

/*получение типа режима*/
Mode ControllerManager::getMode()
{
    return mMode;
}

/*получение типа стрельбы*/
FireType ControllerManager::getFireType()
{
    return mFireType;
}

/*получение нормализованного значения позиции по горизонтали*/
long ControllerManager::getUnitPositionX(long pPosition)
{
    int numberOfRotations = 0;
    long lUnitX = 0;
    numberOfRotations = pPosition / 36000;
    lUnitX = pPosition - numberOfRotations * 36000;
     return lUnitX;
}

/*получение нормализвоанного значения позиции цели*/
long ControllerManager::getDeltaX(long pTargetPosition, long pPosition)
{
    long lDelta = pTargetPosition - pPosition;
    if (lDelta < -18000)
        lDelta += 36000;
    else if (lDelta > 18000)
        lDelta -= 36000;
    return lDelta;
}

/*получение значение юстировки по горизонтали*/
long ControllerManager::getJustX()
{
    long lJustX = 0;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexJust.lock();
    switch (lCameraMode) {
    case WIDE_FORMAT:
        lJustX = mJustCfg[mWeaponType * 4 + 0].JustX;
        break;
    case NARROW_FORMAT:
        lJustX = mJustCfg[mWeaponType * 4 + 1].JustX;
        break;
    case THERMAL:
        lJustX = mJustCfg[mWeaponType * 4 + 2].JustX;
        break;
    case ZOOM_THERMAL:
        lJustX = mJustCfg[mWeaponType * 4 + 3].JustX;
        break;
    }
    mMutexJust.unlock();
    return lJustX;
}

/*получение значение юстировки по вертикали*/
long ControllerManager::getJustY()
{
    long lJustY = 0;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexJust.lock();
    switch (lCameraMode) {
    case WIDE_FORMAT:
        lJustY = mJustCfg[mWeaponType * 4 + 0].JustY;
        break;
    case NARROW_FORMAT:
        lJustY = mJustCfg[mWeaponType * 4 + 1].JustY;
        break;
    case THERMAL:
        lJustY = mJustCfg[mWeaponType * 4 + 2].JustY;
        break;
    case ZOOM_THERMAL:
        lJustY = mJustCfg[mWeaponType * 4 + 3].JustY;
        break;
    }
    mMutexJust.unlock();
    return lJustY;
}

/*получение длительности одного выстрела*/
int ControllerManager::getShotTime()
{
    if (shot_times.find(mWeaponType) != shot_times.end())
        return (shot_times[mWeaponType]);
    else
        return -1;
}

/*команда на взвод вооружения*/
void ControllerManager::addBoltNotch()
{
    Action *action = new Action(ACTION_TYPE_BOLT_NOTCH, 0);
    ActionList::getSingletonPtr()->setAction(action);
    emit newCocking(COCKING_LOAD);
}

/*корректировка перекрестия баллистики в соответствии с дальностью*/
void ControllerManager::correctionToFire()
{
    calculateAmendment();
    long newPositionX = long(atan((double)getCorrectionLeft() / (double)mRange)
                             * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI)
                        + mAmendmentX;
    long newPositionY = long(atan((double)getCorrectionUp() / (double)mRange)
                             * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI)
                        + mAmendmentY;
    long pJustX = 0;
    long pJustY = 0;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexJust.lock();
    switch (lCameraMode) {
    case WIDE_FORMAT:
        pJustX = mJustCfg[mWeaponType * 4 + 0].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 0].JustY;
        break;
    case NARROW_FORMAT:
        pJustX = mJustCfg[mWeaponType * 4 + 1].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 1].JustY;
        break;
    case THERMAL:
        pJustX = mJustCfg[mWeaponType * 4 + 2].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 2].JustY;
        break;
    case ZOOM_THERMAL:
        pJustX = mJustCfg[mWeaponType * 4 + 3].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 3].JustY;
        break;
    }
    mMutexJust.unlock();
    emit newJust(-pJustX, pJustY);
    emit newCorrectionCross(INVERT_SPEED_X * newPositionX,
                            INVERT_SPEED_Y * newPositionY);
}

/*сохранение цели*/
void ControllerManager::saveAim()
{
    // query new range
    mMutexPositionEncoder.lock();
    // add new aim
    mMutexMode.lock();
    if (mMode == MODE_SEMI_AUTO && AimList::getSingletonPtr()->size() < 10) {
        mMutexMode.unlock();
        Aim *lAim = new Aim();
        lAim->mActive = true;
        lAim->mState = true;
        lAim->mPositionX = getUnitPositionX(mPositionEncoderX);
        lAim->mPositionY = mPositionEncoderY;
        lAim->mCorrectionX = getCorrectionLeft();
        lAim->mCorrectionY = getCorrectionUp();
        lAim->mRange = mRange;
        lAim->isAlive = true;
        int lTarget = AimList::getSingletonPtr()->getMaxTarget();
        lTarget++;
        lAim->mTarget = lTarget;
        AimList::getSingletonPtr()->push(lAim);
        emit newLastSaved(lAim->mPositionX, lAim->mPositionY, lAim->mRange, lAim->mTarget);
    } else {
        mMutexMode.unlock();
    }
    mMutexPositionEncoder.unlock();
}

/*сохранение юстировки*/
void ControllerManager::saveJust()
{
    mMutexJust.lock();
    CfgManager::getSingletonPtr()->setJust(mJustCfg);
    mMutexJust.unlock();
}

/*стрельба в соответствии с заданным режимом*/
void ControllerManager::addFire()
{
    mAddFireProcess = true;
    correctionToFire();
    if (mPermission)
    {
        Action *lActionFireType;
        Action *lActionFire;
        mMutexMode.lock();
        mMutexTracking.lock();
        if (mMode == MODE_TRACKING) {
            mMutexTracking.unlock();
            mMutexMode.unlock();
            addFireTracking();
            queryAmmunition();
        }
        else
        {
            mMutexTracking.unlock();
            if (mFireType == TURN_FIRE)
            {
                Action *lActionBurst;
                lActionFireType = new Action(ACTION_TYPE_FIRE_TYPE, 1);
                if (mMode == MODE_SEMI_AUTO)
                {
                    setSpeedDeviceDrive(0, 0);
                    if(!mBlocking)
                    {
                        moveToCorrection();
                    }
                    msleep(50);
                }

                if (mBurstType == LONG_BURST) lActionBurst = new Action(ACTION_TYPE_BURST_LONG,	mBurstLong * 10 * shot_times[mWeaponType]);
                else                          lActionBurst = new Action(ACTION_TYPE_BURST_SHORT,mBurstShort * 10 * shot_times[mWeaponType]);
                //стрельба в режиме сопровождения
                if (enabled_as)
                {
                    setSpeedDeviceDrive(0,0);
                    ActionList::getSingletonPtr()->setAction(lActionBurst);
                    Action *lActionBrake = new Action(ACTION_TYPE_SET_BRAKE, true);
                    ActionList::getSingletonPtr()->setAction(lActionBrake);
                }
                else
                {
                    setSpeedDeviceDrive(spVedX,0/*,spVedY*/);
                    ActionList::getSingletonPtr()->setAction(lActionBurst);
                }
        }
	    else 
        {
            lActionFireType = new Action(ACTION_TYPE_FIRE_TYPE, 0);
            isFire = true;
        }
        emit newFire(true);
        ActionList::getSingletonPtr()->setAction(lActionFireType);
        if(mPermission && !mBlocking)
            {
                lActionFire = new Action(ACTION_TYPE_FIRE, true);
                ActionList::getSingletonPtr()->setAction(lActionFire);
            }
            if (mFireType == TURN_FIRE)
            {
                if (mMode == MODE_SEMI_AUTO)
                {
                    setSpeedDeviceDrive(0, 0);
                    if (mBurstType == LONG_BURST)
                        msleep(mBurstLong * shot_times[mWeaponType]);
                    else
                        msleep(mBurstShort * shot_times[mWeaponType]);
                    msleep(200);
                    Action *lActionBrake = new Action(ACTION_TYPE_SET_BRAKE, false);
                    ActionList::getSingletonPtr()->setAction(lActionBrake);
                    if(!mBlocking)
                    {
                        resetMoveToCorrection();
                    }
                }
                else
                {
                    if (mBurstType == LONG_BURST)  msleep(mBurstLong * shot_times[mWeaponType]);
                    else                           msleep(mBurstShort * shot_times[mWeaponType]);
                    Action *lActionBrake = new Action(ACTION_TYPE_SET_BRAKE, false);
                    ActionList::getSingletonPtr()->setAction(lActionBrake);
                    msleep(200);
                }
                queryAmmunition();
                emit newFire(false);
            }
            mMutexMode.unlock();
        }
    }
    mAddFireProcess = false;
}

/*отмена стрельбы в непрерывном режиме*/
void ControllerManager::resetFire()
{
    mMutexMode.lock();
    if (mFireType != TURN_FIRE && isFire && mMode != MODE_TRACKING) {
        mMutexMode.unlock();
        isFire = false;
        Action *lActionFire = new Action(ACTION_TYPE_FIRE, false);
        ActionList::getSingletonPtr()->setAction(lActionFire);
        emit newFire(false);
        queryAmmunition();
    } else {
        mMutexMode.unlock();
    }
}

/*стрельба в режиме автосопровождения*/
void ControllerManager::addFireTracking()
{
    correctionToFire();
    long lXDeltaCross = long(atan((double)getCorrectionLeft() / (double)mRange)
                             * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI);
    long lYDeltaCross = long(atan((double)getCorrectionUp() / (double)mRange)
                             * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI);
    long pJustX = 0;
    long pJustY = 0;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    mMutexJust.lock();
    switch (lCameraMode) {
    case WIDE_FORMAT:
        pJustX = mJustCfg[mWeaponType * 4 + 0].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 0].JustY;
        break;
    case NARROW_FORMAT:
        pJustX = mJustCfg[mWeaponType * 4 + 1].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 1].JustY;
        break;
    case THERMAL:
        pJustX = mJustCfg[mWeaponType * 4 + 2].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 2].JustY;
        break;
    case ZOOM_THERMAL:
        pJustX = mJustCfg[mWeaponType * 4 + 3].JustX;
        pJustY = mJustCfg[mWeaponType * 4 + 3].JustY;
        break;
    }
    mMutexJust.unlock();
    long lX = pJustX;
    long lY = pJustY;
    long lConvertX = 0;
    long lConvertY = 0;
    switch (lCameraMode) {
    case NARROW_FORMAT:
        lConvertX = static_cast<long>(static_cast<double>(lX)
                                      * ANGLE_NARROW_X
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_WIDTH);
        lConvertY = static_cast<long>(static_cast<double>(lY)
                                      * ANGLE_NARROW_Y
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_HEIGHT);
        break;
    case WIDE_FORMAT:
        lConvertX = static_cast<long>(static_cast<double>(lX)
                                      * ANGLE_WIDE_X
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_WIDTH);
        lConvertY = static_cast<long>(static_cast<double>(lY)
                                      * ANGLE_WIDE_Y
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_HEIGHT);
        break;
    case THERMAL:
        lConvertX = static_cast<long>(static_cast<double>(lX)
                                      * ANGLE_THERMAL_X
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_WIDTH);
        lConvertY = static_cast<long>(static_cast<double>(lY)
                                      * ANGLE_THERMAL_Y
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_HEIGHT);
        break;
    case ZOOM_THERMAL:
        lConvertX = static_cast<long>(static_cast<double>(lX)
                                      * ANGLE_ZOOM_THERMAL_X
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_WIDTH);
        lConvertY = static_cast<long>(static_cast<double>(lY)
                                      * ANGLE_ZOOM_THERMAL_Y
                                      * DEGREE_TO_CENTIDEGREE
                                      / DEFAULT_HEIGHT);
        break;
    }
    lXDeltaCross -= lConvertX;
    lYDeltaCross -= lConvertY;
    long lPosX1 = 0;
    long lPosY1 = 0;
    long Vx = 0;
    long Vy = 0;
    bool statecalculate = false;
    mMutexTracking.lock();
    mTrackingAmendment.setWeaponType(mWeaponType);
    mTrackingAmendment.setAmendmentBallist(mAmendmentX, mAmendmentY);
    mTrackingAmendment.setRange(mRange);
    mTrackingAmendment.setDelta(lXDeltaCross, -lYDeltaCross);
    Action *lActionBurst;
    if (mBurstType == LONG_BURST) {
        mTrackingAmendment.setTimeSpeed((float)mBurstLong * shot_times[mWeaponType] / 1000.f);
        lActionBurst = new Action(ACTION_TYPE_BURST_LONG, mBurstLong * 10 * shot_times[mWeaponType]);
    } else {
        mTrackingAmendment.setTimeSpeed((float)mBurstShort * shot_times[mWeaponType] / 1000.f);
        lActionBurst = new Action(ACTION_TYPE_BURST_SHORT, mBurstShort * 10 * shot_times[mWeaponType]);
    }
    statecalculate = mTrackingAmendment.getPosTargets(lPosX1, lPosY1, Vx, Vy);
    mTrackingCenterX = 0;
    mTrackingCenterY = 0;
    mTracking = false;
    mTypeTracking = 0;
    mMutexTracking.unlock();
    setSpeedDeviceDrive(0, 0);
    emit newFire(true);
    if (statecalculate) {
        if(!mBlocking)
            setPositionDeviceDrive(-lPosX1, lPosY1, true);
        if(!mBlocking)
            setSpeedDeviceDrive(Vx, -Vy);
        Action *lActionFireType = new Action(ACTION_TYPE_FIRE_TYPE, 1);
        ActionList::getSingletonPtr()->setAction(lActionBurst);
        ActionList::getSingletonPtr()->setAction(lActionFireType);
        if(mPermission && !mBlocking){
            Action *lActionFire = new Action(ACTION_TYPE_FIRE, true);
            ActionList::getSingletonPtr()->setAction(lActionFire);
            if (mBurstType == LONG_BURST)
                msleep(mBurstLong * shot_times[mWeaponType]);
            else
                msleep(mBurstShort * shot_times[mWeaponType]);
        }
        setSpeedDeviceDrive(0, 0);
    } else {
        setSpeedDeviceDrive(0, 0);
        emit newLogMsg(tr("Error tracking"));
    }
    mCorrectionTrackX = 0;
    mCorrectionTrackY = 0;
    emit newFire(false);
}

/*включение/выключение подсветки экрана БУ*/
void ControllerManager::switchLight()
{
    bool result;
    if (lightEnabled)
        lightEnabled = false;
    else
        lightEnabled = true;
    result = mControlDevice->switchLight(lightEnabled);
    if (result) {
        if (lightEnabled)
            emit newLogMsg(tr("Backlight on"));
        else
            emit newLogMsg(tr("Backlight off"));
    } else {
        emit newLogMsg(tr("Failed to access backlight"));
    }
}

/*действия при выходе из атоматическо режима*/
void ControllerManager::closeAutoMode()
{
    resetFire();
    mMutexMode.lock();
    mMode = MODE_SEMI_AUTO;
    mMutexMode.unlock();
    isFirstAimByCoord = false;
    mIndexCurrentAim = 0;
    mFireType = mLastFireType;
    newFireType(mFireType);
    emit newAutoMode(true);
    emit newMode(MODE_SEMI_AUTO);
    AimList::getSingletonPtr()->makeAllAimsAlive();
    mControlDevice->clearKeys();
}

/*действия при выходе из режима поход*/
void ControllerManager::closeMarchMode()
{
    mMutexMode.lock();
    mMode = MODE_SEMI_AUTO;
    mMutexMode.unlock();
    emit newMode(MODE_SEMI_AUTO);
    mControlDevice->clearKeys();
}

/*сброс корректировки точки ведения цели в режиме сопровождения*/
void ControllerManager::resetCorrectionTracking()
{
    mCorrectionTrackX = 0;
    mCorrectionTrackY = 0;
    int_part_x = 0;
    diff_part_x = 0;
    int_part_y = 0;
    diff_part_y = 0;
}

/*изменение типа длины очереди*/
void ControllerManager::nextBurstType()
{
    if (mBurstType == SHORT_BURST)
        mBurstType = LONG_BURST;
    else
        mBurstType = SHORT_BURST;
    emit newBurstType(mBurstType);
}

/*изменение типа стрельбы*/
void ControllerManager::nextFireType()
{
    if (mFireType == SINGLE_FIRE) {
        mFireType = TURN_FIRE;
        mLastFireType = TURN_FIRE;
    } else {
        mFireType = SINGLE_FIRE;
        mLastFireType = SINGLE_FIRE;
    }
    emit newFireType(mFireType);
}

/*включение/выключение стабилизации*/
void ControllerManager::nextStabilizationType()
{
    mMutexStabilization.lock();
    if (mStabilization)
        mStabilization = false;
    else
        mStabilization = true;
    Action *action = new Action(ACTION_TYPE_STABILIZATION, mStabilization);
    ActionList::getSingletonPtr()->setAction(action);
    emit newStabilization(mStabilization);
    mMutexStabilization.unlock();
}

/*команда на получение данных датчиков температуры и давления*/
void ControllerManager::querySensors()
{
    Action *lActiongetTermo = new Action(ACTION_TYPE_GET_TRMO_0, 0);
    Action *lActiongetPressure = new Action(ACTION_TYPE_GET_PRESSURE, 0);
    ActionList::getSingletonPtr()->setAction(lActiongetTermo);
    ActionList::getSingletonPtr()->setAction(lActiongetPressure);
}

/*команда на получение данных дальномера*/
void ControllerManager::queryRange()
{
    Action *action = new Action(ACTION_TYPE_GET_RANGE, 0);
    ActionList::getSingletonPtr()->setAction(action);
}
void ControllerManager::queryRange_ac()
{
	Action *action_qRange = new Action (ACTION_TYPE_GET_RANGE, 0);
	ActionList::getSingletonPtr()->setAction(action_qRange);
}

/*команда на получение данных счетчика выстрелов*/
void ControllerManager::queryAmmunition()
{
    Action *lActionAmmunition = new Action(ACTION_TYPE_GET_AMMUNITION_COUNTER, 0);
    ActionList::getSingletonPtr()->setAction(lActionAmmunition);
}

/*включение/выключение стеклоочистителя*/
void ControllerManager::switchWiper(bool is_wiper_working)
{
    Action *action = new Action(ACTION_TYPE_SWITCH_WIPER,
                                static_cast<long>(is_wiper_working));
    ActionList::getSingletonPtr()->setAction(action);
}

/*выполнение действий при закрытии*/
void ControllerManager::close()
{
    isClose = true;
    emit newClose();
    mControlDevice->close();
}

/*команда на запрос данных положения и скорости перемещения ПП*/
void ControllerManager::queryEncoders()
{
    //query position
    Action *lActiongetPosX = new Action(ACTION_TYPE_GET_ENCODER_POSITION_X, 0);
    Action *lActiongetPosY = new Action(ACTION_TYPE_GET_ENCODER_POSITION_Y, 0);
    ActionList::getSingletonPtr()->setAction(lActiongetPosX);
    ActionList::getSingletonPtr()->setAction(lActiongetPosY);
    //query speed
    Action *lActiongetSpeedX = new Action(ACTION_TYPE_GET_ENCODER_SPEED_X, 0);
    Action *lActiongetSpeedY = new Action(ACTION_TYPE_GET_ENCODER_SPEED_Y, 0);
    ActionList::getSingletonPtr()->setAction(lActiongetSpeedX);
    ActionList::getSingletonPtr()->setAction(lActiongetSpeedY);
}

/*команда на запрос напряжения питания ПП*/
void ControllerManager::queryVoltage()
{
    Action *lActiongetVoltageBattery = new Action(ACTION_TYPE_GET_VOLTAGE_BATTERY, 0);
    ActionList::getSingletonPtr()->setAction(lActiongetVoltageBattery);
}

/*установка результата команды на запрос данных положения по горизонтали*/
void ControllerManager::setPositionEncoderX(long pPosition)
{
    mMutexPositionEncoder.lock();
    mPositionEncoderX = pPosition;
    mMutexTracking.lock();
    mTrackingAmendment.setPositionsX(-mPositionEncoderX,
                                     (float)mTracking_res_x);
    mMutexTracking.unlock();
    mMutexPositionEncoder.unlock();
    emit newPositionX(pPosition);
}

/*установка результата команды на запрос данных положения по вертикали*/
void ControllerManager::setPositionEncoderY(long pPosition)
{
    mMutexPositionEncoder.lock();
    mPositionEncoderY = -pPosition;
    mMutexTracking.lock();
    mTrackingAmendment.setPositionsY(-mPositionEncoderY,
                                     (float)mTracking_res_y);
    mMutexTracking.unlock();
    mMutexPositionEncoder.unlock();
    emit newPositionY(pPosition);
}

/*установка результата команды на запрос скорости перемещения по горизонтали*/
void ControllerManager::setSpeedEncoderX(long pSpeed)
{
    mTrackingCenterX_rec = static_cast<long>(static_cast<double>(pSpeed) * 6.0
                                             * DEGREE_TO_CENTIDEGREE / 425.0);
    float speed = static_cast<float>(pSpeed) * 6.0 / 425.0;

    if (pSpeed != 0.0f)
    {
        if (VsrX[0] != 0)
        {
            memmove (VsrX+1,                        /*kyga*/
                     VsrX,                      	/*otkyga*/
                     sizeof(VsrX)-sizeof(VsrX[0]));	/*ckoJlbko bytes*/
            VsrX[0] = pSpeed;
        }
        else for (int i=0; i<100; i++) VsrX[i]=pSpeed;
        for (int i=10; i<35; i++) SrAX += VsrX[i];
        SrAX /= 25.0f;
        //SrAX = SrAX + SrAX*0.07f;
    }
    else for (int i=0; i<100; i++) VsrX[i]=pSpeed;
    VsrXX = static_cast<long>(SrAX);

    switch (mMode) {
    case MODE_SEMI_AUTO:
        emit newSpeedX(speed);
        break;
    case MODE_TRACKING:
        {
            double lim = 0.0;
            switch (mCameraMode) {
            case WIDE_FORMAT:
                lim = 6.0;
                break;
            case NARROW_FORMAT:
                lim = 1.0;
                break;
            case THERMAL:
                lim = 3.0;
                break;
            case ZOOM_THERMAL:
                lim = 1.5;
                break;
            }
            if (fabs(speed) < lim)
                emit newSpeedX(speed);
        }
        break;
    case MODE_MARCH:
        break;
    case MODE_AUTO:
        if (speed != 0.0)
            emit newSpeedX(-speed);
        break;
    }
}

/*установка результата команды на запрос скорости перемещения по вертикали*/
void ControllerManager::setSpeedEncoderY(long pSpeed)
{
    mTrackingCenterY_rec = static_cast<long>(static_cast<double>(pSpeed) * 6.0
                                             * DEGREE_TO_CENTIDEGREE / 512.5);
    float speed = static_cast<float>(pSpeed) * 6.0 / 512.5;

    if (pSpeed != 0.0f)
    {
        if (VsrY[0] != 0)
        {
            memmove (VsrY+1,                        /*kyga*/
                     VsrY,                          /*otkyga*/
                     sizeof(VsrY)-sizeof(VsrY[0])); /*ckoJlbko*/
            VsrY[0] = pSpeed;
        }
        else for (int i=0; i<100; i++) VsrY[i]=pSpeed;
        for (int i=10; i<35; i++) SrAY += VsrY[i];
        SrAY /= 25.0f;
    }
    else for (int i=0; i<100; i++) VsrX[i]=pSpeed;
    VsrYY = static_cast<long>(SrAY);

    switch (mMode) {
    case MODE_SEMI_AUTO:
        emit newSpeedY(speed);
        break;
    case MODE_TRACKING:
        {
            double lim = 0.0;
            switch (mCameraMode) {
            case WIDE_FORMAT:
                lim = 6.0;
                break;
            case NARROW_FORMAT:
                lim = 1.0;
                break;
            case THERMAL:
                lim = 3.0;
                break;
            case ZOOM_THERMAL:
                lim = 1.5;
                break;
            }
            if (fabs(speed) <= lim)
                emit newSpeedY(speed);
        }
        break;
    case MODE_MARCH:
        break;
    case MODE_AUTO:
        if (speed != 0.0)
            emit newSpeedY(speed);
        break;
    }
}

/*команда на установку количества боезапаса*/
void ControllerManager::setAmmunitionCounter(long pAmmunitionCounter, bool is_new)
{
    if (pAmmunitionCounter >= 0 && pAmmunitionCounter < 10000) {
        mAmmunitionCounter = pAmmunitionCounter;
        emit newAmmunitionCounter((int)mAmmunitionCounter);
        if (is_new) {
            Action *action = new Action(ACTION_TYPE_INIT_AMMUNITION_COUNTER,
                                        mAmmunitionCounter );
            ActionList::getSingletonPtr()->setAction(action);
        }
    }
}

/*установка флага подключения ПП*/
void ControllerManager::setConnectedDevice(bool state)
{
    mMutexConnectedDevice.lock();
    isConnectedDevice = state;
    emit setConnectedPlathorm(state);
    mMutexConnectedDevice.unlock();
}

/*установка результата команды на получение данных датчика температуры*/
void ControllerManager::setThermo(long pThermo)
{
    mThermo = pThermo;
    emit newThermo((char)pThermo);
    correctionToFire();
}

/*установка результата команды на получение данных датчика давления*/
void ControllerManager::setPressure(long pPressure, bool pcalculate)
{
    if (pcalculate)
        pPressure = FUN_PRESSURE(pPressure);
    if (pPressure < 1000000 && pPressure > 0) {
        mPressure = pPressure;
        emit newPressure((int)pPressure, pcalculate);
        correctionToFire();
    }
}

/*установка результата команды на получение данных дальномера*/
void ControllerManager::setRange(long pRange)
{
    mMutexRange.lock();
    mRange = pRange;
    winRange = pRange;
    emit newRange(mRange);
    correctionToFire();
    isNewRange = true;
    isCorrectionPosition = true;
    mMutexRange.unlock();
}

/*установка результата команды о достижении заданной позиции*/
void ControllerManager::setAchievementPosition(bool state)
{
    mMutexAchievementPosition.lock();
    isPositionReached = state;
    mMutexAchievementPosition.unlock();
}

/*установка результатов напряжения питания БУ*/
void ControllerManager::setVoltageBu(int pVoltageExternal)
{
    float lVoltageExternal = (float)pVoltageExternal / 10.f;
    if (lVoltageExternal > 0.0f)
        emit newVoltageBu(lVoltageExternal);
}

/*установка результатов команды напряжение питания ПП*/
void ControllerManager::setVoltageExternal(long pVoltage)
{
    float voltage = (float)pVoltage / 1000.0;
    if (voltage > 1)
        emit newVoltageExternal(voltage);
}

/*установка результатов команды взвода вооружения*/
void ControllerManager::setResBoltNotch(int pValue)
{
    setAmmunitionCounter((int)mAmmunitionCounter, true);
    if (pValue == RES_COCKING_COMPLETED) {
        emit newCocking(COCKING_COMPLETED);
    } else {
        emit newCocking(COCKING_ERROR);
    }
    is_load_enabled = true;
}

/*запуск потока обработки событий и джойстика*/
void ControllerManager::run()
{
    correctionToFire();
    setPermission(mPermission);
    while (!need_exit) {
        mMutexConnectedDevice.lock();
        mMutexStoped.lock();
        bool stoped = isStop;
        bool connected = isConnectedDevice;
        mMutexStoped.unlock();
        mMutexConnectedDevice.unlock();
        msleep(2);
        if (connected && !stoped) {
            updateEvent();
            updateJoyKey();
            if (!enabled_as) emit acJoystick();//запуск в потоке на принудительное движение ПП
        } else 
	{
            msleep(2000);
        }
    }
}

/*обработка событий*/
void ControllerManager::updateEvent()
{
    //update joystick
    mJoystick->updateEvent();
    //set new speed

    if (enableJoystick)
    {
        lPosJoyX = mJoystick->getPosX();
        lPosJoyY = mJoystick->getPosY();
    }
    else
    {
        lPosJoyX = 0;
        lPosJoyY =0;
    }


    long lSpeedX = 0;
    long lSpeedY = 0;

    double x = static_cast<double>(abs(lPosJoyX));
    double y = static_cast<double>(abs(lPosJoyY));
    double a_x = 0.0;
    double b_x = 0.0;
    double c_x = 0.0;
    double d_x = 0.0;
    double a_y = 0.0;
    double b_y = 0.0;
    double c_y = 0.0;
    double d_y = 0.0;

    switch (mCameraMode) {
    case WIDE_FORMAT:
        a_x = +4.464411243e-15;
        b_x = -5.566500661e-11;
        c_x = +1.029033145e-06;
        d_x = -1.311591964e-03;
        a_y = +5.409815977e-15;
        b_y = -6.745289037e-11;
        c_y = +1.246946046e-06;
        d_y = -1.589340851e-03;
        break;
    case NARROW_FORMAT:
        a_x = +7.545483807e-16;
        b_x = -9.408170214e-12;
        c_x = +1.739210960e-07;
        d_x = -2.216775189e-04;
        a_y = +9.143350964e-16;
        b_y = -1.140048860e-11;
        c_y = +2.107514455e-07;
        d_y = -2.686209923e-04;
        break;
    case THERMAL:
        a_x = +1.949249981e-15;
        b_x = -2.430443960e-11;
        c_x = +4.492961634e-07;
        d_x = -5.726669190e-04;
        a_y = +2.362032318e-15;
        b_y = -2.945126148e-11;
        c_y = +5.444412250e-07;
        d_y = -6.939375340e-04;
        break;
    case ZOOM_THERMAL:
        a_x = +9.746249906e-16;
        b_x = -1.215221979e-11;
        c_x = +2.246480815e-07;
        d_x = -2.863334589e-04;
        a_y = +1.181061590e-15;
        b_y = -1.472563074e-11;
        c_y = +2.722206123e-07;
        d_y = -3.469687662e-04;
        break;
    }
    lSpeedX = static_cast<long>(a_x * x * x * x * x +
                                b_x * x * x * x +
                                c_x * x * x +
                                d_x * x);
    lSpeedY = static_cast<long>(a_y * y * y * y * y +
                                b_y * y * y * y +
                                c_y * y * y +
                                d_y * y);
    if (lPosJoyX < 0)
        lSpeedX = -lSpeedX;
    if (lPosJoyY < 0)
        lSpeedY = -lSpeedY;

    mMutexTracking.lock();
    if (lSpeedX > 0)
        mCorrectionTrackX += 0.35f;
    else if (lSpeedX < 0)
        mCorrectionTrackX -= 0.35f;
    if (lSpeedY > 0)
        mCorrectionTrackY += 0.35f;
    else if (lSpeedY < 0)
        mCorrectionTrackY -= 0.35f;
    mMutexTracking.unlock();
    mMutexMode.lock();
    Mode lMode = mMode;
    mMutexMode.unlock();
    if (lMode == MODE_SEMI_AUTO) {
        if (!is_load_enabled || mBlocking){
            setSpeedDeviceDrive(0, 0);
            return;
        }
        msleep(5);
        if (!enabled_as || mBlocking)
        {
            if ((!lSpeedX) and (!lSpeedY)){ setSpeedDeviceDrive(pidX   ,  pidY);/* emit newLogMsg("SetX");*/}
            else                           setSpeedDeviceDrive(lSpeedX, -lSpeedY);
        }
        else
        {
            setSpeedDeviceDrive(lSpeedX, -lSpeedY);
        }
    }
    if (lMode == MODE_AUTO){
        if (!is_load_enabled || mBlocking){
            closeAutoMode();
            return;
        }
        resetFire();
        moveToNextAim();
    }
    if (lMode == MODE_TRACKING) {
        if (!is_load_enabled || mBlocking){
        }
        mMutexTracking.lock();
        long lSpeedTrackingX = -mTrackingCenterX;
        long lSpeedTrackingY = mTrackingCenterY;
        mMutexTracking.unlock();
        msleep(10);
        setSpeedDeviceDrive(lSpeedTrackingX, lSpeedTrackingY);
    }
    if (lMode == MODE_MARCH) {
        if (!is_load_enabled || mBlocking){
            closeMarchMode();
            return;
        }
        emit newMode(MODE_MARCH);
        resetFire();
        if(isStabilization()) nextStabilizationType();
        moveToMarch();
    }
}

/*обновление сотояния функциональной клавиатуры*/
void ControllerManager::updateKey()
{
    Key pKeyPressed;
    bool state = mControlDevice->getKeyPressed(pKeyPressed);
    if (!state)
        return;
    switch (pKeyPressed.id) {
    case KEY_AIM_MODE:
        if (pKeyPressed.state == 1) {
            if (!enabled_as) {
            switch (mAimType) {
            case HOME:
                mAimType = CAR;
                break;
            case CAR:
                mAimType = HOME;
                break;
           }
            emit newAimType(mAimType);
           }
        }
        break;
    case KEY_SENSORS:
        if (is_load_enabled && pKeyPressed.state == 1) {
            querySensors();
            emit newReset();
        }
        break;
    case KEY_TYPE_BURST:
        if (pKeyPressed.state == 1)
            if (mFireType == TURN_FIRE) {
                //если установлен режим по очередям
                //меняем тип очереди
                nextBurstType();
                emit newReset();
            }
        break;
    case KEY_BOLT_NOTCH:
        if (getMode()==MODE_SEMI_AUTO && !mBlocking && is_load_enabled && pKeyPressed.state == 1) {
            setSpeedDeviceDrive(0, 0);
            addBoltNotch();
            emit newReset();
            is_load_enabled = false;
        }
        break;
    case KEY_PERMISSION:
        if (pKeyPressed.state == 0){
            setPermission(false);
            resetFire();
        }
        else
            setPermission(true);
        emit newReset();
        break;
    case KEY_FIRE_TYPE:
        if (pKeyPressed.state == 1)
            nextFireType();
        emit newReset();
        break;
    case KEY_STABILIZATION:
        if(!mBlocking || isStabilization())
            if (is_load_enabled && pKeyPressed.state == 1) {
            mMutexMode.lock();
            if (mMode != MODE_AUTO && mMode != MODE_MARCH)
                nextStabilizationType();
            mMutexMode.unlock();
        }
        emit newReset();
        break;
    case KEY_SAVE_AIM:
        if (!mBlocking && is_load_enabled && pKeyPressed.state == 1)
            saveAim();
        emit newReset();
        break;
    case KEY_INVERT:
        if (pKeyPressed.state == 1)
            emit invertCrossColor();
        emit newReset();
        break;
    case KEY_AUTO:
        if (mBlocking || !is_load_enabled || AimList::getSingletonPtr()->size() <= 0)
            break;
        mMutexStabilization.lock();
        mMutexTracking.lock();
        mMutexMode.lock();
        if (pKeyPressed.state == 1 && mMode != MODE_TRACKING) {
            if (mAutoType == AUTO_TYPE_COORDINATE) {
                AimList::getSingletonPtr()->sortAims();
                shotCounter = 0;
            }
            isFirstAimByCoord = true;
            emit newMode(MODE_AUTO);
            mMode = MODE_AUTO;
        }
        mMutexMode.unlock();
        mMutexTracking.unlock();
        mMutexStabilization.unlock();
        emit newReset();
        break;
    case KEY_TRACKING:
        if (!mBlocking && is_load_enabled && pKeyPressed.state == 1)
        {
            if (enabled_as)
            {
                enabled_as = false;
                emit newTrack(true);
            }
            else
            {
                enabled_as = true;
                emit newTrack(false);
                emit closePID();
            }
        }
        emit newReset();
        break;
    }
}

/*обновление состояния кнопок джойстика*/
void ControllerManager::updateJoyKey()
{
    int lKey = 0;
    bool state = mJoystick->getKeyPress(lKey);
#ifdef AVATAR_SDELAL
    if((state_range==1)&&(lKey==KEY_RANGE)) //BLOCK GEt Range
    {
        state=false;
    }
    if((state_range==1)&&(timer_range<=0))//OFF BLOCK
    {
        state_range=0; timer_range=0;
    }
    if(state_range==1) timer_range--;//refresh time

    if((state_range==0)&&((state==true)&&(lKey==KEY_RANGE)))//get range
    {
        state_range=1; timer_range=1000;
    }
 #endif
    if (state)
    {
        mMutexCamera.lock();
        mMutexCamera.unlock();
        switch (lKey)
        {
            case KEY_CAMERA_WIDE_FORMAT:
                if (fKey == lKey)       break;
                    emit setCamera_AC(WIDE_FORMAT);
		mCameraMode = WIDE_FORMAT;
                fKey = lKey;
                correctionToFire();
                emit newReset();
                break;
            case KEY_CAMERA_NARROW_FORMAT:
                if (fKey == lKey)
                    break;
                fKey = lKey;
                emit setCamera_AC(NARROW_FORMAT);
                mCameraMode = NARROW_FORMAT;
                correctionToFire();
                emit newReset();
                break;
            case KEY_CAMERA_THERMAL:
                if (fKey == lKey)
                    break;
                fKey = lKey;
                emit setCamera_AC(THERMAL);
		mCameraMode = THERMAL;
                correctionToFire();
                emit newReset();
                break;
            case KEY_CAMERA_ZOOM_THERMAL:
                if (fKey == lKey)
                    break;
                fKey = lKey;
                emit setCamera_AC(ZOOM_THERMAL);
		mCameraMode = ZOOM_THERMAL;
                correctionToFire();
                emit newReset();
                break;
            case KEY_FIRE:
                if (mKeyState->keyFire)
                    break;
                mKeyState->keyFire = true;
                addFire();
                emit newReset();
                break;
            case KEY_RANGE:
            {
               #ifdef AVATAR_SDELAL

                mDeviceThread->take_range=0;
               #endif
                if (mKeyState->keyRange)
                    break;
                mKeyState->keyRange = true;
                queryRange();
                emit newReset();
            }
                break;
        }
     }
    else
    {
         if (mKeyState->keyFire)
         {
             mKeyState->keyFire = false;
             resetFire();
             emit newReset();
         }
         mKeyState->keyRange = false;
     }
}

/*обновление сотояния функциональной клавиатуры по сигналу таймера*/
void  ControllerManager::updateKeys()
{
    bool stoped, connected;

    mMutexConnectedDevice.lock();
    mMutexStoped.lock();
    stoped = isStop;
    connected = isConnectedDevice;
    mMutexStoped.unlock();
    mMutexConnectedDevice.unlock();

    if (connected && !stoped) {
        updateKey();
    }
}

/*рассчет баллистических поправок*/
void  ControllerManager::calculateAmendment()
{
    Ballistics lBalistics;
    mMutexCamera.lock();
    mMutexCamera.unlock();
    if (lBalistics.calculate(mWeaponType, mRange, mThermo,
                             mPressure, mWindX, mWindY)) {
        mAmendmentX = lBalistics.getAmendmentX();
        mAmendmentY = lBalistics.getAmendmentY();
    } else {
        mAmendmentX = 0;
        mAmendmentY = 0;
        emit newLogMsg(tr("Ballistic tables file not found"));
    }
}

/*команда на получение результата о достижении заданной позиции*/
void ControllerManager::queryAchievementPosition()
{
    Action *action = new Action(ACTION_TYPE_ACHIEVEMENT_POSITION, 0);
    ActionList::getSingletonPtr()->setAction(action);
}

/*перемещение по целям во время срельбы в атоматическом режиме*/
void ControllerManager::moveToNextAim()
{
    Aim *lAim;
    //***********************************************************************
    //auto type by order
    //***********************************************************************
    if (mAutoType == AUTO_TYPE_ORDER) {
        lAim = AimList::getSingletonPtr()->get(mIndexCurrentAim);
        if (lAim) {
            //correction to fire
            if (lAim->mActive && lAim->isAlive) {
                //new range aim
                mMutexRange.lock();
                mRange = lAim->mRange;
                emit newRange(mRange);
                correctionToFire();
                mMutexRange.unlock();
                long correctionPositionX = -INVERT * (long(atan(lAim->mCorrectionX / lAim->mRange)
                                                * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentX);
                long correctionPositionY = INVERT * (long(atan(lAim->mCorrectionY / lAim->mRange)
                                                * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentY);
                long DeltaPositionX = 0;
		long UnitPositionX = 0;
                UnitPositionX = getUnitPositionX(mPositionEncoderX);
                DeltaPositionX = -getDeltaX(lAim->mPositionX, UnitPositionX);
                setPositionDeviceDrive(-DeltaPositionX - correctionPositionX,
                                       (mPositionEncoderY - lAim->mPositionY
                                        - correctionPositionY), true);
                mFireType = TURN_FIRE;
                emit newFireType(TURN_FIRE);
                msleep(200);
                if (mPermission && !mBlocking)
                    addFire();
                else
                    resetFire();
                msleep(200);
                lAim->isAlive = false;
            }
            mIndexCurrentAim++;
        } else {
            closeAutoMode();
        }
    }
    //***********************************************************************
    //auto type by coordinats
    //***********************************************************************
    if (mAutoType == AUTO_TYPE_COORDINATE) {
        long DeltaPositionX = 0;
        long UnitPositionX = 0;
        UnitPositionX = getUnitPositionX(mPositionEncoderX);
        if (shotCounter < AimList::getSingletonPtr()->sizeActive()) {
            lAim = AimList::getSingletonPtr()->getAimSortNext(shotCounter);
            ++shotCounter;
            mMutexRange.lock();
            mRange = lAim->mRange;
            emit newRange(mRange);
            correctionToFire();
            mMutexRange.unlock();
            long correctionPositionX = -INVERT * (long(atan(lAim->mCorrectionX
                                                            / lAim->mRange)
                                                       * 180.0
                                                       * DEGREE_TO_CENTIDEGREE
                                                       / M_PI)
                                                  + mAmendmentX);
            long correctionPositionY = INVERT * (long(atan(lAim->mCorrectionY
                                                           / lAim->mRange)
                                                      * 180.0
                                                      * DEGREE_TO_CENTIDEGREE
                                                      / M_PI)
                                                 + mAmendmentY);
            DeltaPositionX = -getDeltaX(lAim->mPositionX, UnitPositionX);
            setPositionDeviceDrive(-DeltaPositionX - correctionPositionX,
                                   (mPositionEncoderY - lAim->mPositionY
                                    - correctionPositionY), true);
            //fire
            emit newFireType(TURN_FIRE);
            mFireType = TURN_FIRE;
            msleep(200);
            if (mPermission && !mBlocking)
                addFire();
            else
                resetFire();
            msleep(200);
            lAim->isAlive = false;
        } else {
            closeAutoMode();
        }
    }
}

/*установка ПП в нулевое положение*/
void ControllerManager::moveToMarch()
{
    setMoveToZeroDeviceDrive();
    closeMarchMode();
}

/*перемещение ПП на основании коррекции по видеокамерам и баллистике*/
//968
void ControllerManager::moveToCorrection()
{
    if (!enabled_as)
    {
        uprAngX = koffupr * uprezhd_prec(mRange,VsrXX,true);
        uprAngY = koffupr * uprezhd_prec(mRange,VsrYY,true);
        uprMetX = koffupr * uprezhd_prec(mRange,VsrXX,false);
        uprMetY = koffupr * uprezhd_prec(mRange,VsrYY,false);
        if ((pidX < 0) && (uprAngX < 0)) uprAngX *= -1.0;
        if ((pidX > 0) && (uprAngX > 0)) uprAngX *= -1.0;
        if ((pidY < 0) && (uprAngY < 0)) uprAngY *= -1.0;
        if ((pidY > 0) && (uprAngY > 0)) uprAngY *= -1.0;

        if (!f_uprGor) {uprAngX = 0; uprMetX = 0;}
        if (!f_uprVer) {uprAngY = 0; uprMetY = 0;}
    }
    else
    {
        uprAngX = 0.0;
        uprAngY = 0.0;
    }
    long newPositionX = long(atan((double)getCorrectionLeft() / (double)mRange) * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentX + uprAngX;
    long newPositionY = long(atan((double)getCorrectionUp()   / (double)mRange) * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentY + uprAngY;
    setPositionDeviceDrive(INVERT_SPEED_X * newPositionX, INVERT_SPEED_Y * newPositionY, true);
    if (!enabled_as) emit acRamkaFreez(false);
    msleep(100);
    if (!enabled_as) emit acRamkaFreez(false);

}

/*возвращение ПП на исходное место на основании коррекции по видеокамерам и баллистике*/
void ControllerManager::resetMoveToCorrection()
{
    long newPositionX = long(atan((double)getCorrectionLeft() / (double)mRange) * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentX + uprAngX;
    long newPositionY = long(atan((double)getCorrectionUp()   / (double)mRange) * 180.0 * DEGREE_TO_CENTIDEGREE / M_PI) + mAmendmentY + uprAngY;
    setPositionDeviceDrive(-INVERT_SPEED_X * newPositionX, -INVERT_SPEED_Y * newPositionY, false);
    if (!enabled_as) 
	{
		emit acRamkaFreez(false);
		for (int i=0; i<5; i++) 
		{
			emit acRamkaFreez(true);
			msleep(100);
		}
	}
}
double ControllerManager::uprezhd(long daln, long vsr)
{
    double v = 0.0;
    double ang = 0.0;
    int xDaln = 0;
    int xVsr = 0;


   v = vsr * 60.0 / 4250 / 360 * 2 * M_PI * daln * 3.6; //км/ч

    for (int i=0; i<20; i++)
    {
        long ix = (i+1)*100;
        if (ix < daln)
        {
            xDaln = i+1;
            if (xDaln > 19) xDaln = 19;
        }
        else
        {
            for (int j=0; j<6; j++)
            {
                long jx = (j+1)*10;
                if (jx < v)
                {
                    xVsr = j+1;
                    if (xVsr > 5) xVsr = 5;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    ang = atan(tabl_upr[xDaln][xVsr] / (double)mRange) * 180.0 / M_PI * 100.0;
    return ang;
}

double ControllerManager::uprezhd_prec(long daln, long vsr, bool aOm)
{
    double v = 0.0;
    double ang = 0.0;
    int xDaln = 0;
    //int xVsr = 0;
    int xSp = 0;
    double x1,y1,x2,y2;
    x1=x2=y1=y2=0;
    double spRez;
    double upr;
    // х - расстояние до цели
    // y - скорость пули
    

    v = vsr * 60.0 / 4250 / 360 * 2 * M_PI * daln * 3.6; //км/ч

    for (int i=0; i<20; i++)
    {
        long ix = (i+1)*100;
        if (ix <= daln)
        {
            xDaln = i;
            xSp   = i+1;
            if (xDaln > 19) xDaln = 19;
            if (xSp > 20) xSp = 20;
            x1 = ix;
            x2 = ix + 100;
        }
        else
        {
            if (i == 0)
            {
                x1 = 0; x2 = 100;
            }
            y1 = tabl_spBullet [xSp]; //начало прямой
            if (xSp > 20) y2 = tabl_spBullet [xSp]; //конец прямой
            else y2 = tabl_spBullet [xSp+1]; //конец прямой
            break;
        }
    }

    if (mRange < 2000) spRez = (x2*y1 - x1*y2 - mRange*y1 + mRange*y2) / (x2-x1);
    else spRez = 446;
    spBullet = spRez;
    //return spRez;
    upr = (v * mRange)/spRez / 3.6;
    if (aOm) ang = atan(upr / (double)mRange) * 180 / M_PI * 100.0; //угол
    else ang = upr;                                         //метры
    return ang;
}

void ControllerManager::init_tabl_upr()
{
/*[метры][км\ч]*/            /*10*/                  /*20*/                  /*30*/                  /*40*/                  /*50*/                  /*60*/
/*[метры][узлы]*/             /*5.4*/                /*10.8*/                /*16.2*/                /*21.6*/                /*27*/                  /*32.4*/

/* 100*/    tabl_upr[ 0][0] = 0.36; tabl_upr[ 0][1] = 0.72; tabl_upr[ 0][2] = 1.10; tabl_upr[ 0][3] = 1.40; tabl_upr[ 0][4] = 1.80; tabl_upr[ 0][5] = 2.10;
/* 200*/    tabl_upr[ 1][0] = 0.72; tabl_upr[ 1][1] = 1.40; tabl_upr[ 1][2] = 2.20; tabl_upr[ 1][3] = 2.90; tabl_upr[ 1][4] = 3.60; tabl_upr[ 1][5] = 4.30;
/* 300*/    tabl_upr[ 2][0] = 1.10; tabl_upr[ 2][1] = 2.20; tabl_upr[ 2][2] = 3.30; tabl_upr[ 2][3] = 4.40; tabl_upr[ 2][4] = 5.60; tabl_upr[ 2][5] = 6.60;
/* 400*/    tabl_upr[ 3][0] = 1.50; tabl_upr[ 3][1] = 3.10; tabl_upr[ 3][2] = 4.50; tabl_upr[ 3][3] = 6.00; tabl_upr[ 3][4] = 7.60; tabl_upr[ 3][5] = 9.10;
/* 500*/    tabl_upr[ 4][0] = 1.90; tabl_upr[ 4][1] = 3.90; tabl_upr[ 4][2] = 5.80; tabl_upr[ 4][3] = 7.70; tabl_upr[ 4][4] = 9.70; tabl_upr[ 4][5] = 11.9;

/* 600*/    tabl_upr[ 5][0] = 2.40; tabl_upr[ 5][1] = 4.80; tabl_upr[ 5][2] = 7.20; tabl_upr[ 5][3] = 9.50; tabl_upr[ 5][4] = 11.9; tabl_upr[ 5][5] = 14.5;
/* 700*/    tabl_upr[ 6][0] = 2.90; tabl_upr[ 6][1] = 5.70; tabl_upr[ 6][2] = 8.60; tabl_upr[ 6][3] = 11.5; tabl_upr[ 6][4] = 14.3; tabl_upr[ 6][5] = 17.3;
/* 800*/    tabl_upr[ 7][0] = 3.40; tabl_upr[ 7][1] = 6.70; tabl_upr[ 7][2] = 10.1; tabl_upr[ 7][3] = 13.5; tabl_upr[ 7][4] = 16.8; tabl_upr[ 7][5] = 20.1;
/* 900*/    tabl_upr[ 8][0] = 3.90; tabl_upr[ 8][1] = 7.80; tabl_upr[ 8][2] = 11.7; tabl_upr[ 8][3] = 13.6; tabl_upr[ 8][4] = 19.4; tabl_upr[ 8][5] = 23.3;
/*1000*/    tabl_upr[ 9][0] = 4.50; tabl_upr[ 9][1] = 8.90; tabl_upr[ 9][2] = 13.4; tabl_upr[ 9][3] = 17.9; tabl_upr[ 9][4] = 22.2; tabl_upr[ 9][5] = 26.8;

/*1100*/    tabl_upr[10][0] = 5.10; tabl_upr[10][1] = 10.1; tabl_upr[10][2] = 15.2; tabl_upr[10][3] = 20.3; tabl_upr[10][4] = 25.3; tabl_upr[10][5] = 30.3;
/*1200*/    tabl_upr[11][0] = 5.70; tabl_upr[11][1] = 11.4; tabl_upr[11][2] = 17.2; tabl_upr[11][3] = 22.9; tabl_upr[11][4] = 28.6; tabl_upr[11][5] = 34.3;
/*1300*/    tabl_upr[12][0] = 6.40; tabl_upr[12][1] = 12.8; tabl_upr[12][2] = 19.3; tabl_upr[12][3] = 25.7; tabl_upr[12][4] = 32.1; tabl_upr[12][5] = 38.9;
/*1400*/    tabl_upr[13][0] = 7.20; tabl_upr[13][1] = 14.3; tabl_upr[13][2] = 21.5; tabl_upr[13][3] = 28.7; tabl_upr[13][4] = 35.8; tabl_upr[13][5] = 43.0;
/*1500*/    tabl_upr[14][0] = 8.00; tabl_upr[14][1] = 15.9; tabl_upr[14][2] = 23.8; tabl_upr[14][3] = 31.9; tabl_upr[14][4] = 39.7; tabl_upr[14][5] = 47.7;

/*1600*/    tabl_upr[15][0] = 8.80; tabl_upr[15][1] = 17.6; tabl_upr[15][2] = 26.3; tabl_upr[15][3] = 35.1; tabl_upr[15][4] = 43.8; tabl_upr[15][5] = 52.7;
/*1700*/    tabl_upr[16][0] = 9.60; tabl_upr[16][1] = 19.3; tabl_upr[16][2] = 28.9; tabl_upr[16][3] = 38.5; tabl_upr[16][4] = 48.1; tabl_upr[16][5] = 57.7;
/*1800*/    tabl_upr[17][0] = 10.5; tabl_upr[17][1] = 21.1; tabl_upr[17][2] = 31.6; tabl_upr[17][3] = 42.1; tabl_upr[17][4] = 52.7; tabl_upr[17][5] = 63.3;
/*1900*/    tabl_upr[18][0] = 11.5; tabl_upr[18][1] = 23.0; tabl_upr[18][2] = 34.4; tabl_upr[18][3] = 45.9; tabl_upr[18][4] = 57.5; tabl_upr[18][5] = 69.0;
/*2000*/    tabl_upr[19][0] = 12.4; tabl_upr[19][1] = 24.9; tabl_upr[19][2] = 37.3; tabl_upr[19][3] = 49.8; tabl_upr[19][4] = 62.2; tabl_upr[19][5] = 74.8;

    tabl_spBullet [0 ] = 820.0; //0 //выход из ствола

    tabl_spBullet [1 ] = 782.6; //37,4
    tabl_spBullet [2 ] = 773.4; //9.2
    tabl_spBullet [3 ] = 752.5; //20.9
    tabl_spBullet [4 ] = 740.7; //11.8
    tabl_spBullet [5 ] = 736.6; //4.1

    tabl_spBullet [6 ] = 692.0; //44.6
    tabl_spBullet [7 ] = 672.0; //20.0
    tabl_spBullet [8 ] = 658.4; //13.6
    tabl_spBullet [9 ] = 642.4; //16.0
    tabl_spBullet [10] = 619.6; //22.8

    tabl_spBullet [11] = 602.1; //17.5
    tabl_spBullet [12] = 583.9; //18.2
    tabl_spBullet [13] = 560.5; //23.4
    tabl_spBullet [14] = 541.3; //19.2
    tabl_spBullet [15] = 522.4; //18.9

    tabl_spBullet [16] = 505.5; //16.9
    tabl_spBullet [17] = 491.5; //14.0
    tabl_spBullet [18] = 475.1; //16.4
    tabl_spBullet [19] = 458.9; //16.2
    tabl_spBullet [20] = 446.8; //12.1
}

/*получение коррекции по вертикали для соответствующей видеокамеры*/
float ControllerManager::getCorrectionUp()
{
    float value = 0.0f;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    switch (lCameraMode) {
    case NARROW_FORMAT:
        value = CORRECTION_POSITION_UP_NARROW;
        break;
    case WIDE_FORMAT:
        value = CORRECTION_POSITION_UP_WHITE;
        break;
    case THERMAL:
        value = CORRECTION_POSITION_UP_HEAD;
        break;
    case ZOOM_THERMAL:
        value = CORRECTION_POSITION_UP_HEAD;
        break;
    }
    return value;
}

/*получение коррекции по горизонтали для соответствующей видеокамеры*/
float ControllerManager::getCorrectionLeft()
{
    float value = 0.0f;
    mMutexCamera.lock();
    CameraMode lCameraMode = mCameraMode;
    mMutexCamera.unlock();
    switch (lCameraMode) {
    case NARROW_FORMAT:
        value = CORRECTION_POSITION_LEFT_NARROW;
        break;
    case WIDE_FORMAT:
        value = CORRECTION_POSITION_LEFT_WHITE;
        break;
    case THERMAL:
        value = CORRECTION_POSITION_LEFT_HEAD;
        break;
    case ZOOM_THERMAL:
        value = CORRECTION_POSITION_LEFT_HEAD;
        break;
    }
    return value;
}



/*чтение длительности одного выстрела*/
bool ControllerManager::readShotTimes()
{
    int kord;
    int pkt;
    int ags;
    int ag40;

    std::fstream i_file;
    i_file.open(SHOT_TIME_CFG, std::fstream::in);
    if (i_file.is_open()) {
        i_file >> kord >> pkt >> ags >> ag40;
        if (i_file.rdstate() % (std::fstream::failbit)) {
            kord = pkt = 70;
            ags = ag40 = 170;
            is_shot_times_changed = true;
        } else {
            is_shot_times_changed = false;
        }
        i_file.close();
    } else {
        kord = pkt = 70;
        ags = ag40 = 170;
        is_shot_times_changed = true;
    }

    shot_times.clear();
    shot_times.insert(std::pair<WeaponType, int>(WEAPON_KORD, kord));
    shot_times.insert(std::pair<WeaponType, int>(WEAPON_PKT, pkt));
    shot_times.insert(std::pair<WeaponType, int>(WEAPON_AGS, ags));
    shot_times.insert(std::pair<WeaponType, int>(WEAPON_AG40, ag40));

    return true;
}

/*запись длительности одного выстрела*/
bool ControllerManager::writeShotTimes()
{
    if (!is_shot_times_changed)
        return true;

    if (shot_times.empty()) {
        return false;
    }

    std::fstream o_file;
    o_file.open(SHOT_TIME_CFG, std::fstream::out | std::fstream::trunc);
    if (!o_file.is_open()) {
        return false;
    }

    o_file << shot_times[WEAPON_KORD] << std::endl
           << shot_times[WEAPON_PKT] << std::endl
           << shot_times[WEAPON_AGS] << std::endl
           << shot_times[WEAPON_AG40]<< std::endl;

    o_file.close();

    return true;
}

/*установка флага выхода*/
void ControllerManager::stopExecution()
{
    need_exit = true;
}
