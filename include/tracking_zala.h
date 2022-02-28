#ifndef TRACKINGZALA_H
#define TRACKINGZALA_H

#include "pid.h"
#include "canad.h"
#include "qsimpleserver.h"
#include "controllermanager.h"
#include "optwidget.h"
#include "videowidget.h"

class TrackingZala : public QThread
{
    Q_OBJECT
public:
    explicit TrackingZala(QWidget *parent = 0);
    ~TrackingZala();

    void acTrack(bool pFlag);
    void setCameraModeAC(CameraMode pCameraMode);
    void setPID(PID *pPID);
    void setControllerManager(ControllerManager *pControllerManager);
    void setOptWidget(OptWidget *pOptWidget);
    void setVideoWidget(VideoWidget *pVideoWidget);
    void upMove();
    void downMove();
    void rightMove();
    void leftMove();
    void normalMode();
    void pictureInPicture();
    void smallPictureInPicture();
    void horizontalLine();
    void verticalLine();
    void digitalStabilization();
    void digitalStabilizationOFF();
    void inversiaModeAc(bool pFlag);
    bool f_on_900, f_send900;
    float VsrX[100];
    float VsrY[100];//массив

private:
    float angleAir(double spX, double spY, double spR);

    PID *mPIDx, *mPIDy, *mPID;
    ControllerManager *mControllerManager;
    QSimpleServer *server;
    OptWidget *mOptWidget;
    VideoWidget *mVideoWidget;

    can_frame txf;
    QTimer *timerD, *mTimer, *filtrTimer, *rangeTimer;

    bool flag_acTrack, t_indi_ac;
    bool indikator_ac, flag_konstr, ramka, f_ramkaFreez;
    int16_t gm0, gmx1, gmy1, gb0, gbx1, gby1;
    int16_t xZala, yZala;

    double spMotorX, spMotorY, spRezult;
    float spAngle;


    float SrAX;
    float SrAY;
    int ViX;
    int ViY;


signals:
    void newLogMsg(QString pString);
    void queryRange_ac_to_Cm();

public slots:
    void move_Ac_Target(long x, long y);
    void setAimType(AimType pAimtype);

private slots:
    void ac_Joystick();
    void AC_Joystick();
    void readVPx (float pv);
    void readVPy (float pv);
    void acRamkaFreez(bool move);
    void getKoordinatAc(can_frame *frame);
    void filterIndikator();
    void time_indi_ac();
    void queryRange_ac();


};

#endif // TRACKINGZALA_H
