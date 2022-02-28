#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QValidator>
#include <QLabel>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "adjustment_dialog.h"
#include "burstlength_dialog.h"
#include "ammunition_dialog.h"
#include "automodetype_dialog.h"
#include "burstlength_dialog.h"
#include "soundsignal_dialog.h"
#include "targeteditor_dialog.h"

#include "videowidget.h"
#include "optwidget.h"
#include "plathormdir.h"
#include "controllermanager.h"
#include "action.h"
#include "actionlist.h"
#include "weapon.enum.h"
#include "aim.enum.h"
#include "cfgmanager.h"
#include "beeper.h"

#include "tracking_zala.h"

#include "joystick.h"
#include "pid.h"
#include "qsimpleserver.h"
#include <qendian.h>

enum MessageType {
    neutral,
    safe,
    unsafe
};

namespace Ui {
    class MainWindow;
}

Q_DECLARE_METATYPE(CameraMode)
Q_DECLARE_METATYPE(BurstType)
Q_DECLARE_METATYPE(FireType)
Q_DECLARE_METATYPE(Mode)
Q_DECLARE_METATYPE(Cocking)
Q_DECLARE_METATYPE(AimType)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
//ZALA
    void acTrack(bool enabled); //Включение режима сопровождения
    void setCameraMode_AC(CameraMode pCameraMode);
//ZALA

    void setSpeedX(float pSpeed);
    void setSpeedY(float pSpeed);

    void setPositionX(long pPosition);
    void setPositionY(long pPosition);

    void setVoltageBu(float pVoltageExternal);
    void setThermo(int pThermo);
    void setPressure(int pPressure);
    void setVoltage(float pVoltage);

    void appendLog(QString pStr);
    void setRange(int pRange);
    void setAmmunitionCounter(int pAmmunitionCounter);
    void setPermission(bool state);
    void setBurstType(BurstType pBurstType);
    void setFireType(FireType pFireType);
    void setFire(bool state);
    void setCorrectionCross(long pCorrectionX, long pCorrectionY);
    void setJustCross(long pJutsX, long pJutsY);
    void setStabilization(bool state);
    void setMode(Mode pMode);
    void setCocking(Cocking pCocking);
    void setLastSaved(long pPositionX, long pPositionY, int pRange, int pNumberAim);
    void setCameraMode(CameraMode pCameraMode);
    void setAutoMode(bool state);
    void resetBeep();
    void testBeep(BeepState pBeepState);
    void setBeepState(BeepState pBeepState);
    void closeAll();
    void setActivatePlathorm(bool activate);
    void on_actionAmmunition_triggered();
    void on_actionBurst_length_triggered();
    void on_rangeInput_editingFinished();
    void on_actionStand_by_mode_triggered();
    void on_actionOperation_mode_triggered();
    void on_windXInput_editingFinished();
    void on_windYInput_editingFinished();
    void on_temperatureInput_editingFinished();
    void on_pressureInput_editingFinished();
    void on_actionClose_triggered();


protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    ControllerManager *mControllerManager;
    VideoWidget *mVideoWidget;
    OptWidget   *mOptWidget;
    PlathormDir *mPlathormDir;
    SoundSignalDialog *mSoundSignalDialog;
    Beeper      *mBeeper;
    can_frame txf;
    Joystick *mJoystick;
    QSimpleServer *server;
    QTimer *mTimer, *filtrTimer, *timerD;
    PID *mPIDx, *mPIDy, *mPID;

	TrackingZala *mTrackingZala;
    bool test,test1;

    AdjustmentDialog ad;
    BurstLengthDialog mBurstLengthDialog;
    AutoModeTypeDialog mAutoModeTypeDialog;
    QMovie *movie;

    bool isVideo;
    int  mLogId;
    int  mTypeTracking;
    bool isOptRange;
    bool isFilter;
    int  mLogID;
    bool isTracking;
    bool is_low_cu_voltage;
    bool is_low_rp_voltage;
    bool is_inverted_enable;
    bool is_ctrl_pressed;
    bool is_s_pressed;
    bool is_t_pressed;
    int  mPositionMouseX;
    int  mPositionMouseY;
    bool isBeepIpa;
    bool is_first_beep;
    bool is_cocking_in_progress;
    bool is_wiper_working;
    CameraMode cur_camera;
    WeaponType cur_weapon;

    bool invers,inversia_stabilization;
    bool inversia_picture;
    bool flagInversia;
    bool flagActivate;
    bool flagPid;

    bool is_p;
    bool is_i;
    bool is_d;

    void setMessage(QLabel *label, QString msg = "",
                    enum MessageType m_type = neutral);
    void setInterfaceInteractivity(bool interactive);

    float angleAir(double spX, double spY, double spR);

    void paintNormal();
    void paintInversia();

	void setConnectedDevice(bool state);
	void setLoadingVideo(bool state);
	void setOperationMode(bool state);
	
private slots:
    void showSoundSignalDialog();
    void showAdjustmentDialog();
    void enableInterface();
    void resetCamera();

//Zala
    void on_actionNormal_mode_triggered();
    void on_actionPicture_in_picture_triggered();
    void on_actionSmall_picture_in_picture_triggered();
    void on_actionHorizontal_line_triggered();
    void on_actionVertical_line_triggered();
    void on_actionDigital_stabilization_triggered();

    void on_actionPID_triggered();

    void inversia(bool pFlag);

    void on_inversiaButton_clicked();
    void on_actionKORD_triggered();
    void on_actionPKT_triggered();

    void on_actionAGS_17_triggered();

    void on_actionAGS_40_triggered();

    void on_sliderUpr_actionTriggered(int action);

    void on_sliderUpr_sliderMoved(int position);

    void on_koffLeft_clicked();

    void on_koff1_clicked();

    void on_koffRight_clicked();

signals:
    void readPageX(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8, float b9);
    void readPageY(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8, float b9);

};

#endif // MAINWINDOW_H
