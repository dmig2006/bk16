#ifndef CONTROLLER_MANAGER
#define CONTROLLER_MANAGER
/*****************************************************************************/
/*** controller                                                            ***/
/*****************************************************************************/
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QTime>

#include <iostream>
#include <map>
#include <cmath>

#include "cfgmanager.h"
#include "supportmanager.h"
#include "controldevice.h"
#include "aimlist.h"
#include "aim.h"
#include "weapon.enum.h"
#include "aim.enum.h"
#include "actionlist.h"
#include "action.h"
#include "joystick.h"
#include "devicethread.h"
#include "ballistics.h"
#include "tracking.h"
#include "devicethread.h"
#include "view.enum.h"
//#include "tracking_zala.h"


#ifndef DEGREE_TO_CENTIDEGREE
#define DEGREE_TO_CENTIDEGREE   100.0
#endif

#ifndef MINUTE_TO_CENTIDEGREE
#define MINUTE_TO_CENTIDEGREE   (100.0 / 60.0)
#endif

#define DEGREE_TO_ENCODER_X     773.6
#define DEGREE_TO_ENCODER_Y     182.04

#define MINUTA_TO_ENCODER_X     12.91
#define MINUTA_TO_ENCODER_Y     3.034

//vert +70
#define CORRECTION_POSITION_LEFT_NARROW     0.340
#define CORRECTION_POSITION_UP_NARROW      -0.02
#define CORRECTION_POSITION_LEFT_HEAD       0.4345
#define CORRECTION_POSITION_UP_HEAD         0.007
#define CORRECTION_POSITION_LEFT_WHITE      0.340
#define CORRECTION_POSITION_UP_WHITE       -0.111

#define MAX_VEL_HOR     153.8
#define MAX_VEL_VERT    44.2

#define MAX_SPEED_NARROW_FORMAT_X    100  // 1 degree
#define MAX_SPEED_NARROW_FORMAT_Y    100
#define MAX_SPEED_WIDE_FORMAT_X      600  // 6 degrees
#define MAX_SPEED_WIDE_FORMAT_Y      600
#define MAX_SPEED_THERMAL_X          300  // 3 degrees
#define MAX_SPEED_THERMAL_Y          300
#define MAX_SPEED_ZOOM_THERMAL_X     150  // 1.5 degrees
#define MAX_SPEED_ZOOM_THERMAL_Y     150

#define RES_COCKING_COMPLETED   15

#define FUN_PRESSURE(a)         (int)(((double)a - 410) / 3276.0 * 30 * 57.715 - 77);

#define INVERT_SPEED_Y	 1
#define INVERT_SPEED_X	-1

#define INVERT -1

#define SHOT_TIME_CFG    "config/shot_times.cfg"

//COLOR NORMA
#define COLOR_FON_NORMA                         "background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                    stop: 0 #c8c2c2, stop: 1 #7f7a7a);"
//LABEL
#define COLOR_LABEL_NORMA                       "QLabel\
                                                 {\
                                                    background-color:disabled;\
                                                    color:#000000;\
                                                 }"

#define COLOR_LABEL_NORMA_DATA                  "QLabel\
                                                 {\
                                                    background-color: disabled;\
                                                    color:#000000;\
                                                 }"

#define COLOR_LABEL_NORMA_ACTIVATE              "QLabel\
                                                 {\
                                                    background-color:disabled;\
                                                 }"
//SLIDER
#define COLOR_SLIDER_NORMA                      "QSlider\
                                                 {\
                                                    background-color:disabled;\
                                                 }"
//LINE
#define COLOR_LINE_NORMA                         "background-color:disabled;"

//PUSHBUTTON
#define COLOR_BUTTON_NORMA_ACTIVATE             "QPushButton\
                                                 {\
                                                    background-color: #D4D4D4;\
                                                    color:#000000;\
                                                    border: 2px outset #a0a0a0;\
                                                    border-radius: 4px;\
                                                    font size: 12px;\
                                                 }\
                                                 QPushButton:pressed\
                                                 {\
                                                    background-color: #D4D4D4;\
                                                    color:#000000;\
                                                    border: 2px inset #a0a0a0;\
                                                    border-radius: 4px;\
                                                 }"


#define PICTURE_NIGHT_BUTTON                  "QPushButton#inversiaButton\
                                               {\
                                                  background-color: #D4D4D4;\
                                                  image: url(://res/images/Moon.png);\
                                                  border: 2px outset #a0a0a0;\
                                                  border-radius: 4px;\
                                               }\
                                               QPushButton#inversiaButton:pressed\
                                               {\
                                                  background-color: #D4D4D4;\
                                                  image: url(://res/images/Moon.png);\
                                                  border: 2px inset #a0a0a0;\
                                                  border-radius: 4px;\
                                               }"

//EDIT
#define COLOR_EDIT_NORMA_ACTIVATE               "background-color:#FFFFFF; color:#000000"

#define COLOR_EDIT_SOUND                        "background-color:#FFFFFF; color:#000000"

//LogWidget
#define COLOR_LIST_WIDGET_NORMA                 "background-color:#FFFFFF; color:#000000;"

//QMenu
#define COLOR_MENU_NORMA                        "QMenu::item{background-color:disabled; #D4D4D4; color:#610404;}\
                                                 QMenu::item:selected{background-color:#9CC75B;}"

#define COLOR_MENU_BAR_NORMA                    "QMenuBar::item{background-color:#D4D4D4; color:#610404;}\
                                                 QMenuBar::item:selected{background-color:#9CC75B}\
                                                 QMenuBar#menuBar{background-color: disabled;}"

//RADIO BUTTON
#define COLOR_RADIO_BUTTON_NORMA_HORIZONTAL     "QRadioButton::indicator {width: 10px; height:10px;}\
                                                 QRadioButton::indicator::unchecked {background-color:#FFFFFF;}\
                                                 QRadioButton::indicator::checked {background-color:#610404;}\
                                                 QRadioButton#byHorizontalCoordinateRadioButton{background-color:disabled;color:#000000}"

#define COLOR_RADIO_BUTTON_NORMA_SAVE           "QRadioButton::indicator {width: 10px; height:10px;}\
                                                 QRadioButton::indicator::unchecked {background-color:#FFFFFF;}\
                                                 QRadioButton::indicator::checked {background-color:#610404;}\
                                                 QRadioButton#inSavingOrderRadioButton{background-color:disabled;color:#000000}"

//List
#define COLOR_LIST_NORMA                        "QListWidget::indicator {width: 10px; height:10px;}\
                                                 QListWidget::indicator::unchecked {background-color:#958E95;}\
                                                 QListWidget::indicator::checked {background-color:#610404;}\
                                                 QListWidget::item:selected{background-color:#A6A2A5;}\
                                                 QListWidget#listWidget{background-color:#FFFFFF;color:#610404}"

//COLOR NORMA

//COLOR INVERSIA
//FON
#define COLOR_FON_INVERSIA                      "background-color:#000000"

//Label
#define COLOR_LABEL_INVERSIA                    "color:#FFFFFF"

#define COLOR_LABEL_INVERSIA_DATA               "QLabel\
                                                 {\
                                                    background-color:disabled;\
                                                    color:#F68080;\
                                                  }"
//SLIDER
#define COLOR_SLIDER_INVERSIA                   "QSlider\
                                                 {\
                                                    background-color:disabled;\
                                                  }"
//LINE
#define COLOR_LINE_INVERSIA                     "background-color:#858585"


//QPushButton
#define COLOR_BUTTON_INVERSIA_ACTIVATE          "QPushButton\
                                                 {\
                                                    background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                       stop: 0 #353434, stop: 1 #2b2929);\
                                                    color:#ECD2D2;\
                                                    border: 2px outset #858585;\
                                                    border-radius: 4px;\
                                                 }\
                                                 QPushButton:pressed\
                                                 {\
                                                    background-color: #989197;\
                                                    color:#ECD2D2;\
                                                    border: 2px inset #858585;\
                                                    border-radius: 4px;\
                                                 }"

#define PICTURE_DAY_BUTTON                    "QPushButton#inversiaButton\
                                               {\
                                                    background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                 stop: 0 #353434, stop: 1 #2b2929);\
                                                    image: url(://res/images/Sun.png);\
                                                    border: 2px outset #858585;\
                                                    border-radius: 4px;\
                                               }\
                                               QPushButton#inversiaButton:pressed\
                                               {\
                                                    background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                stop: 0 #353434, stop: 1 #2b2929);\
                                                    border: 2px inset #858585;\
                                                    border-radius: 4px;\
                                               }"


//TEXT EDIT
#define COLOR_EDIT_INVERSIA_ACTIVATE            "QLineEdit\
                                                 {\
                                                    background:qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                      stop: 0 #353434, stop: 1 #2b2929);\
                                                    border-radius: 4px;\
                                                    color:#ECD2D2;\
                                                 }"

//LIST WIDGET
#define COLOR_LIST_WIDGET_INVERSIA              "background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                stop: 0 #353434, stop: 1 #2b2929);\
                                                 color:#ECD2D2;\
                                                 border-radius:6px"

//QMenu
#define COLOR_MENU_INVERSIA                     "background-color: #000000; color:#FFFFFF; selection-background-color: #989197; selection-color:#ECD2D2"

#define COLOR_MENU_BAR_INVERSIA                 "QMenuBar::item{background-color:#000000; color:#FFFFFF;}\
                                                 QMenuBar::item:selected{background-color:#989197;color:#ECD2D2}\
                                                 QMenuBar#menuBar{background-color:#000000}"
//RADIO BUTOON
#define COLOR_RADIO_BUTTON_INVERSIA_HORIZONTAL  "QRadioButton::indicator {width: 10px; height:10px;}\
                                                 QRadioButton::indicator::unchecked {background-color:#989197;}\
                                                 QRadioButton::indicator::checked {background-color:#157704;}\
                                                 QRadioButton#byHorizontalCoordinateRadioButton{background-color:#000000;color:#FFFFFF;}"

#define COLOR_RADIO_BUTTON_INVERSIA_SAVE        "QRadioButton::indicator {width: 10px; height:10px;}\
                                                 QRadioButton::indicator::unchecked {background-color:#989197;}\
                                                 QRadioButton::indicator::checked {background-color:#157704;}\
                                                 QRadioButton#inSavingOrderRadioButton{background-color:#000000;color:#FFFFFF;}"


//LIST
#define COLOR_LIST_INVERSIA                     "QListWidget::indicator {width: 10px; height:10px;}\
                                                 QListWidget::indicator::unchecked {background-color:#FFFFFF;}\
                                                 QListWidget::indicator::checked {background-color:#157704;}\
                                                 QListWidget::item:selected{background-color:#111111;color:#F68080;}\
                                                 QListWidget#listWidget\
                                                 {\
                                                    background-color:qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                                  stop: 0 #353434, stop: 1 #2b2929);\
                                                    color:#ECD2D2\
                                                 }"

//COLOR INVERSIA



class ControllerManager : public QThread
{
    Q_OBJECT

public:

#ifdef AVATAR_SDELAL

    int count_range; //счетчик вызова события GET_RANGE

    /**
     * @brief state_range
     * 1 - событие отправлено ответа нет еще
     * 2 - получено отрицательное значение
     *
     */
    int state_range; //состояния
    int timer_range;

#endif

    ControllerManager();
    ~ControllerManager();
    void closeDevice();
    void closeSocket();

    void setPause(bool state);
    void setMode(Mode pMode);
    void setStandbyMode();
    void setOperationMode();
    void setStopDeviceDrive();
    void setMoveToZeroDeviceDrive();
    void setPositionDeviceDrive(long pPositionX, long pPositionY,
                                bool waitAchievementPosition);
    void setSpeedDeviceDrive(long pSpeedX, long pSpeedY);
    void setTracking(bool enabled);
    void setZoomTermo(int pState);
    void setBurstType(BurstType burst_type);

    void setJust(long pJustX, long pJustY);
    void setCorrectionTracking(int x, int y);
    void setFireType(FireType fire_type);
    void setCenterMassTracking(int x, int y);
    void setVelocityTracking(float x, float y);
    void setWindX(long pWindX);
    void setWindY(long pWindY);
    void setWeapon(WeaponType  pWeaponType);
    void setShortBurst(long pBurstShort);
    void setLongBurst(long pBurstShort);
    void setTypeTracking(int pType);
    void setPermission(bool state);
    void setBlocking(bool state);
    void setAutoType(AutoType pAutoType);
    void setDisplayLastAim(Aim *lAim);
    void setFlagEsc();
    void setCamera(CameraMode pCameraMode);
    void setShotTime(int s_time);
    //get metods
    Mode getMode();
    FireType getFireType();
    long getUnitPositionX(long pPosition);
    long getDeltaX(long pTargetPosition, long pPosition);
    long getJustX();
    long getJustY();
    long getPositionEncoderX() const { return mPositionEncoderX; }
    long getPositionEncoderY() const { return mPositionEncoderY; }
    long getAmmunitionCounter() const { return mAmmunitionCounter; }
    long getShortBurst() const { return mBurstShort; }
    long getLongBurst() const { return mBurstLong; }
    bool getPermission() const { return mPermission; }
    bool getBlocking() const { return mBlocking; }
    BurstType getBurstType() const { return mBurstType; }
    AutoType getAutoType() const { return mAutoType; }
    int getTypeTracking() const { return mTypeTracking; }
    WeaponType getWeaponType() const { return mWeaponType; }
    CameraMode getCamera() { return mCameraMode; }
    int getShotTime();
    int getAimsSize() const { return AimList::getSingletonPtr()->size(); }

    bool isStabilization() const { return mStabilization; }
    bool isTracking() const { return mTracking; }
    bool isAddFireProcess() const { return mAddFireProcess;}

    void addBoltNotch();
    void correctionToFire();
    void saveAim();
    void saveJust();
    void addFire();
    void resetFire();
    void addFireTracking();
    void switchStabilization();
    void switchLight();
    void closeAutoMode();
    void closeMarchMode();
    void resetCorrectionTracking();
    void nextBurstType();
    void nextFireType();
    void nextStabilizationType();
    void querySensors();
    void queryRange();
    void queryRange_ac();
    void queryAmmunition();
    void switchWiper(bool is_wiper_working);

    void moveToMarch();

    //Joystick----------------------------
    bool         enableJoystick;
    //Zala Aero-------------------------------
    bool enabled_as;
    long pidX,pidY;
    bool mPermission;
    //Joystick-------------------------------
     long lPosJoyX;
     long lPosJoyY;
     //упреждение
     long spVedX,spVedY;
     long VsrXX,VsrYY;
     double uprAngX;
     double uprAngY;
     double uprMetX;
     double uprMetY;
     double winRange;
     double koffupr;
     float VsrX[100];
     float VsrY[100];
     bool f_uprVer;
     bool f_uprGor;
     //упреждение

public slots:
    void close();
    void queryEncoders();
    void queryVoltage();
    void setPositionEncoderX(long pPosition);
    void setPositionEncoderY(long pPosition);
    void setSpeedEncoderX(long pSpeed);
    void setSpeedEncoderY(long pSpeed);
    void setAmmunitionCounter(long pAmmunitionCounter, bool is_new);
    void setConnectedDevice(bool state);
    void setThermo(long pThermo);
    void setPressure(long pPressure, bool pcalculate);
    void setRange(long pRange);
    void setAchievementPosition(bool state);
    void setVoltageBu(int pVoltageExternal);
    void setVoltageExternal(long pVoltage);
    void setResBoltNotch(int pValue);
    void stopExecution();
    void updateKeys();

protected:
    void run();

private:
    void updateEvent();
    void updateKey();
    void updateJoyKey();
    void calculateAmendment();
    void queryAchievementPosition();
    void moveToNextAim();
    void moveToCorrection();
    void resetMoveToCorrection();
    float getCorrectionUp();
    float getCorrectionLeft();

    //упреждение
    double uprezhd (long daln, long vsr);
    double uprezhd_prec (long daln, long vsr, bool aOm);
    double tabl_upr [20][6];
    double tabl_spBullet [21];
    double spBullet;
    void init_tabl_upr();

    float SrAX;
    float SrAY;
    int ViX;
    int ViY;
    //упреждение

    bool readShotTimes();
    bool writeShotTimes();

    AimType       mAimType;
    ControlDevice *mControlDevice;
    DeviceThread  *mDeviceThread;
    Joystick*     mJoystick;
    float         mCorrectionTrackX;
    float         mCorrectionTrackY;
    bool          flagEsc;
    //thread values---------------------------
    bool          isClose;
    QMutex        mMutexPositionEncoder;
    QMutex        mMutexPermission;
    QMutex        mMutexBlocking;
    QMutex        mMutexAchievementPosition;
    QMutex        mMutexAutoType;
    QMutex        mMutexMode;
    QMutex        mMutexRange;
    QMutex        mMutexConnectedDevice;
    QMutex        mMutexStoped;
    QMutex        mMutexStabilization;
    QMutex        mMutexTracking;
    QMutex        mMutexJust;
    QMutex        mMutexCamera;

    bool          isConnectedDevice;
    bool          lightEnabled;
    //current speed---------------------------
    long         mCurrentSpeedX;
    long         mCurrentSpeedY;

    //current position-----------------------
    long         mCurrentPositionX;
    long         mCurrentPositionY;
    long         mPositionEncoderX;
    long         mPositionEncoderY;
    bool         isPositionReached;
    bool         isCorrectionPosition;

    long         mTrackingCenterX;
    long         mTrackingCenterY;

    long         mTrackingCenterX_rec;
    long         mTrackingCenterY_rec;

    long         mTracking_res_x;
    long         mTracking_res_y;

    int          mTrackingX;
    int          mTrackingY;

    long         mCorrectionX;
    long         mCorrectionY;

    long         mRange;
    bool         isNewRange;

    float        int_part_x;
    float        diff_part_x;
    float        int_part_y;
    float        diff_part_y;

    JoyKeyState*     mKeyState;

    //ammution and fire----------------------
    long         mAmmunitionCounter;
    bool         isnewAmmunitionCounter;
    //Burst length
    long         mBurstShort;
    long         mBurstLong;
    BurstType    mBurstType;
    FireType     mFireType;
    AutoType     mAutoType;
    WeaponType   mWeaponType;
    Mode         mMode;
    bool         mBlocking;
    bool         mAddFireProcess;
    bool         isResetFire;
    bool         isFire;
    bool         isFirstAimByCoord;
    bool         isStop;

    JustCfg      mJustCfg[16];

    long         mWindX;
    long         mWindY;

    long         mAmendmentX;
    long         mAmendmentY;

    //tracking------------------------------
    bool         mTracking;
    int          mTypeTracking; // 0 - detector, 1 - match
    Tracking     mTrackingAmendment;
    QTimer*      mTimerEncoder;
    QTimer*      mTimerKey;

    //stabilization-------------------------
    bool         mStabilization;

    //valtage-------------------------------
    int          mVoltageExternalBu;
    int          mVoltageBatteryBu;

    //aims----------------------------------
    int          mIndexCurrentAim;

    long         mThermo;
    long         mPressure;

    //ZALA
    int fKey;

    CameraMode   mCameraMode;
    struct timespec time_s;
    struct timespec time_n;

    FireType     mLastFireType;

    long         mSpeedTrackingSmoothX;
    long         mSpeedTrackingSmoothY;

    bool is_load_enabled;
    int shotCounter;

    std::map<WeaponType, int> shot_times;
    bool is_shot_times_changed;
    bool need_exit;

signals:
    void newRange(int range);
    void newJust(long justX, long justY);
    void newCorrectionCross(long x, long y);
    void newSpeedX(float x);
    void newSpeedY(float y);
    void newPositionX(long position);
    void newPositionY(long position);
    void newVoltageBu(float voltageBu);
    void newThermo(int thermo);
    void newPressure(int pressure, bool calc);
    void newVoltageExternal(float voltageExternal);
    void newPermission(bool state);
    void newBlocking(bool state);
    void newAmmunitionCounter(int ammunitionCounter);
    void newFire(bool state);
    void newBurstType(BurstType burstType);
    void newFireType(FireType fireType);
    void newStabilization(bool state);
    void newLastSaved(long positionX, long positionY, int range, int numberAim);
    void newAutoMode(bool state);
    void newMode(Mode mode);
    void newLogMsg(QString log);
    void newCameraMode(CameraMode cameraMode);
    void newClose();
    void newTrackig();
    void newCocking(Cocking cocking);
    void newReset();
    void newAimType(AimType aimType);
    void setConnectedPlathorm(bool activate);
    void invertCrossColor();

    //ZALA_AS
     void newTrack(bool enabled);
     void setCamera_AC(CameraMode cameraMode);

     void acJoystick();
     void acRamkaFreez(bool move);
    //ZALA_AS

     void closePID();
};

#endif // CONTROLLER_MANAGER
