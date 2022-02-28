#include "tracking_zala.h"

TrackingZala::TrackingZala(QWidget *parent) : QThread(parent)
{
    mPIDx = new PID;
    mPIDy = new PID;
    mPIDx  -> setGeometry(820,0,180,690);
    mPIDy  -> setGeometry(820,0,180,690);

    server = new QSimpleServer;
    server->start();

    mPIDx -> pid.Pkff_z0=8.00f  ;mPIDy -> pid.Pkff_z0=8.00f;
    mPIDx -> pid.Ikff_z0=80.00f ;mPIDy -> pid.Ikff_z0=80.00f;
    mPIDx -> pid.Dkff_z0=350.00f;mPIDy -> pid.Dkff_z0=350.00f;

    mPIDx -> pid.Pkff_z1=8.00f  ;mPIDy -> pid.Pkff_z1=8.00f;
    mPIDx -> pid.Ikff_z1=80.00f ;mPIDy -> pid.Ikff_z1=80.00f;
    mPIDx -> pid.Dkff_z1=350.00f;mPIDy -> pid.Dkff_z1=350.00f;

    mPIDx -> pid.Pkff_z2=8.00f  ;mPIDy -> pid.Pkff_z2=8.00f;
    mPIDx -> pid.Ikff_z2=80.00f ;mPIDy -> pid.Ikff_z2=80.00f;
    mPIDx -> pid.Dkff_z2=350.00f;mPIDy -> pid.Dkff_z2=350.00f;

    mPIDx -> pid.Zona = 0.0f;    mPIDy -> pid.Zona = 0.0f;
    mPIDx -> pid.MaxU = 4250;    mPIDy -> pid.MaxU = 4250;
    mPIDx -> pid.MinU = -4250;   mPIDy -> pid.MinU = -4250;
    mPIDx -> pid.PidV_stop = 50; mPIDy -> pid.PidV_stop = 50;

    xZala= 0;   yZala= 0;
    gm0  = 32;  gb0  = 64;
    gmx1 = 158; gbx1 = 158;
    gmy1 = 135; gby1 = 135;

    timerD = new QTimer; //Таймер дискретизации пид
    mTimer = new QTimer; // Таймер для пакетов 900
    filtrTimer = new QTimer; //Пакеты 1210
    rangeTimer = new QTimer; //Таймер измерения дальности с автоматом сапровождения

    flag_acTrack = false;
    t_indi_ac = false;

    indikator_ac = false; //флаг для просмотра включения слежения v
    flag_konstr = false;
    ramka = false;  //флаг размера рамки f-большая, t-малая
    f_ramkaFreez = true;
    f_on_900 = false;
    f_send900= false;

    spMotorX = 0;
    spMotorY = 0;
    spRezult = 0;
    spAngle = 0.0f;

    SrAX = 0.0f;
    SrAY = 0.0f;
    ViX = 0;
    ViY = 0;

    connect(server, SIGNAL(newLogMsg(QString)), SIGNAL(newLogMsg(QString)));
    connect(server, SIGNAL(FrameReady(can_frame*)), this, SLOT(getKoordinatAc(can_frame*)));

}

TrackingZala::~TrackingZala()
{
    delete mPIDx;
    delete mPIDy;
    delete server;

    delete timerD;
    delete mTimer;
    delete filtrTimer;
    delete rangeTimer;
}

void TrackingZala::acTrack(bool pFlag)
{
    if (pFlag)
    {
        memset(VsrX,0.0f,sizeof(VsrX));
        memset(VsrY,0.0f,sizeof(VsrY));
        SrAX = 0.0f;
        SrAY = 0.0f;

        timerD -> start(static_cast<int>(mPIDx -> pid.Tmls));

        mOptWidget -> flagLabel = true;

        flag_acTrack = true;
        txf.can_dlc = 4;        //Длина поля данных в байтах
        txf.can_id  = 1213;  //0x05e5 функция включения адресс
        txf.data[0] = 0b00000001;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00000000;
        txf.data[3] = 0b00000000; //qToBigEndian(0xa1b2c3d4);
        server -> FramesToBytes(&txf);
        connect (mControllerManager, SIGNAL(acJoystick()), this, SLOT(ac_Joystick())); //Если подается сигнал acJoystick(), посылаем пакеты 900
        connect (mControllerManager, SIGNAL(acRamkaFreez(bool)), this, SLOT(acRamkaFreez(bool)));
    }
    else
    {
        mOptWidget -> flagLabel = false;

        txf.can_dlc = 4;        //Длина поля данных в байтах
        txf.can_id  = 1213;  //0x05e5 функция включения адресс
        txf.data[0] = 0b00000000;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00000000;
        txf.data[3] = 0b00000000;
        server -> FramesToBytes(&txf);

        t_indi_ac = false;
        if (mControllerManager -> enabled_as)
        {
            if (flag_acTrack)
            {
                mPIDx -> pid_on_off(false, 0);
                mPIDy -> pid_on_off(false, 0);
                connect(mPIDx, SIGNAL(readValuePid(float)), this, SLOT(readVPx(float)));
                connect(mPIDy, SIGNAL(readValuePid(float)), this, SLOT(readVPy(float)));
                mTimer -> stop();
                timerD -> stop();
                rangeTimer -> stop();
                filtrTimer -> stop();
                flag_acTrack = false;
                mControllerManager -> enabled_as = true;
            }
        }
    }
}

/*Выбор режима камеры*/
void TrackingZala::setCameraModeAC(CameraMode pCameraMode)
{
    switch (pCameraMode)
    {
    case WIDE_FORMAT:
        mOptWidget -> setCameraMode(pCameraMode);
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1211;
        //txf.data = 0x11120002;
        txf.data[0] = 0b00000010;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00010010;
        txf.data[3] = 0b00010001;
        f_send900 = true;
        AC_Joystick();

        mPIDx -> pid.Pkff_z0=8.00f  ;mPIDy -> pid.Pkff_z0=8.00f;
        mPIDx -> pid.Ikff_z0=80.00f ;mPIDy -> pid.Ikff_z0=80.00f;
        mPIDx -> pid.Dkff_z0=350.00f;mPIDy -> pid.Dkff_z0=350.00f;

        mPIDx -> pid.Pkff_z1=8.00f  ;mPIDy -> pid.Pkff_z1=8.00f;
        mPIDx -> pid.Ikff_z1=80.00f ;mPIDy -> pid.Ikff_z1=80.00f;
        mPIDx -> pid.Dkff_z1=350.00f;mPIDy -> pid.Dkff_z1=350.00f;

        mPIDx -> pid.Pkff_z2=8.00f  ;mPIDy -> pid.Pkff_z2=8.00f;
        mPIDx -> pid.Ikff_z2=80.00f ;mPIDy -> pid.Ikff_z2=80.00f;
        mPIDx -> pid.Dkff_z2=350.00f;mPIDy -> pid.Dkff_z2=350.00f;

        break;

    case NARROW_FORMAT:
        mOptWidget -> setCameraMode(pCameraMode);
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1211;
        //txf.data = 0x11120001;
        txf.data[0] = 0b00000001;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00010010;
        txf.data[3] = 0b00010001;
        f_send900 = true;
        AC_Joystick();

        mPIDx->pid.Pkff_z0= 1.20f  ;mPIDy->pid.Pkff_z0=1.20f;
        mPIDx->pid.Ikff_z0=1000.0f ;mPIDy->pid.Ikff_z0=1000.0f;
        mPIDx->pid.Dkff_z0=150.00f;mPIDy->pid.Dkff_z0=150.00f;

        mPIDx->pid.Pkff_z1=1.20f  ;mPIDy->pid.Pkff_z1=1.20f;
        mPIDx->pid.Ikff_z1=1000.0f ;mPIDy->pid.Ikff_z1=1000.0f;
        mPIDx->pid.Dkff_z1=150.00f;mPIDy->pid.Dkff_z1=150.00f;

        mPIDx->pid.Pkff_z2=1.20f  ;mPIDy->pid.Pkff_z2=1.20f;
        mPIDx->pid.Ikff_z2=1000.0f ;mPIDy->pid.Ikff_z2=1000.00f;
        mPIDx->pid.Dkff_z2=150.00f;mPIDy->pid.Dkff_z2=150.00f;

        break;

    case THERMAL:
        mOptWidget -> setCameraMode(pCameraMode);
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1211;
        //txf.data = 0x11120003
        txf.data[0] = 0b00000011;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00010010;
        txf.data[3] = 0b00010001;
        mControllerManager -> setZoomTermo(0);
        f_send900 = true;
        AC_Joystick();

        mPIDx->pid.Pkff_z0= 1.20f  ;mPIDy->pid.Pkff_z0=1.20f;
        mPIDx->pid.Ikff_z0=1000.0f ;mPIDy->pid.Ikff_z0=1000.0f;
        mPIDx->pid.Dkff_z0=150.00f;mPIDy->pid.Dkff_z0=150.00f;

        mPIDx->pid.Pkff_z1=1.20f  ;mPIDy->pid.Pkff_z1=1.20f;
        mPIDx->pid.Ikff_z1=1000.0f ;mPIDy->pid.Ikff_z1=1000.0f;
        mPIDx->pid.Dkff_z1=150.00f;mPIDy->pid.Dkff_z1=150.00f;

        mPIDx->pid.Pkff_z2=1.20f  ;mPIDy->pid.Pkff_z2=1.20f;
        mPIDx->pid.Ikff_z2=1000.0f ;mPIDy->pid.Ikff_z2=1000.00f;
        mPIDx->pid.Dkff_z2= 150.00f;mPIDy->pid.Dkff_z2=150.00f;

        break;

    case ZOOM_THERMAL:
        mOptWidget -> setCameraMode(pCameraMode);
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1211;
        //txf.data = 0x11120003
        txf.data[0] = 0b00000011;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00010010;
        txf.data[3] = 0b00010001;        
        mControllerManager -> setZoomTermo(1);
        f_send900 = true;
        AC_Joystick();

        mPIDx->pid.Pkff_z0=1.20f  ;mPIDy->pid.Pkff_z0=1.20f;
        mPIDx->pid.Ikff_z0=1000.0f ;mPIDy->pid.Ikff_z0=1000.0f;
        mPIDx->pid.Dkff_z0=150.00f;mPIDy->pid.Dkff_z0=150.00f;

        mPIDx->pid.Pkff_z1=1.20f  ;mPIDy->pid.Pkff_z1=1.20f;
        mPIDx->pid.Ikff_z1=1000.0f ;mPIDy->pid.Ikff_z1=1000.0f;
        mPIDx->pid.Dkff_z1=150.00f;mPIDy->pid.Dkff_z1=150.00f;

        mPIDx->pid.Pkff_z2=1.20f  ;mPIDy->pid.Pkff_z2=1.20f;
        mPIDx->pid.Ikff_z2=1000.0f ;mPIDy->pid.Ikff_z2=1000.00f;
        mPIDx->pid.Dkff_z2=150.00f;mPIDy->pid.Dkff_z2=150.00f;

        break;
    }
    server ->FramesToBytes(&txf);
}

void TrackingZala::setPID(PID *pPID)
{
    mPID = pPID;
    mPID -> znachenieStart( mPIDx -> pid.Zona,     mPIDy -> pid.Zona,
                            mPIDx -> pid.Tmls,     mPIDy -> pid.Tmls,
                            mPIDx -> pid.Pves,     mPIDy -> pid.Pves,
                            mPIDx -> pid.Dves,     mPIDy -> pid.Dves,
                            mPIDx -> pid.MaxU,     mPIDy -> pid.MaxU,
                            mPIDx -> pid.MinU,     mPIDy -> pid.MinU,
                            mPIDx -> pid.Pkff_z0,  mPIDy -> pid.Pkff_z0,
                            mPIDx -> pid.Ikff_z0,  mPIDy -> pid.Ikff_z0,
                            mPIDx -> pid.Dkff_z0,  mPIDy -> pid.Dkff_z0,
                            mPIDx -> pid.fTmls  ,  mPIDy -> pid.fTmls,
                            mPIDx -> pid.Pkff_z1,  mPIDy -> pid.Pkff_z1,
                            mPIDx -> pid.Ikff_z1,  mPIDy -> pid.Ikff_z1,
                            mPIDx -> pid.Dkff_z1,  mPIDy -> pid.Dkff_z1,
                            mPIDx -> pid.Pkff_z2,  mPIDy -> pid.Pkff_z2,
                            mPIDx -> pid.Ikff_z2,  mPIDy -> pid.Ikff_z2,
                            mPIDx -> pid.Dkff_z2,  mPIDy -> pid.Dkff_z2);
    //абсолютное перемещение рамки
}

void TrackingZala::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
}

void TrackingZala::setOptWidget(OptWidget *pOptWidget)
{
    mOptWidget = pOptWidget;
}

void TrackingZala::setVideoWidget(VideoWidget *pVideoWidget)
{
    mVideoWidget = pVideoWidget;
}

/*Перемещение относительно центра рамки, по стрелки вверх*/
void TrackingZala::upMove()
{
    if (!(mControllerManager -> enabled_as))
    {
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1212;  //0x05e5 функция включения адресс
        txf.data[0] = static_cast<int8_t>(0); //коордианата x постоянна
        txf.data[1] = static_cast<int8_t>(1); //координата y изменяется на +1
        txf.data[2] = 0x00;
        txf.data[3] = 0x80;
        server -> FramesToBytes(&txf);
    }
}

/*Перемещение относительно центра рамки, по стрелке вниз*/
void TrackingZala::downMove()
{
    if (!(mControllerManager -> enabled_as))
    {
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1212;  //0x05e5 функция включения адресс
        txf.data[0] = static_cast<int8_t>(0); //коордианата x постоянна
        txf.data[1] = static_cast<int8_t>(-1); //координата y изменяется на -1
        txf.data[2] = 0x00;
        txf.data[3] = 0x80;
        server -> FramesToBytes(&txf);
    }
}

/*Перемещение относительно центра рамки,по стрелке вправо*/
void TrackingZala::rightMove()
{
    if (!(mControllerManager -> enabled_as))
    {
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1212;  //0x05e5 функция включения адресс
        txf.data[0] = static_cast<int8_t>(1); //коордианата x изменяется на +1
        txf.data[1] = static_cast<int8_t>(0); //координата y постоянна
        txf.data[2] = 0x00;
        txf.data[3] = 0x80;
        server -> FramesToBytes(&txf);
    }
}

/*Перемещение относительно центра рамки,по стрелке влево*/
void TrackingZala::leftMove()
{
    if (!(mControllerManager -> enabled_as))
    {
        txf.can_dlc = 4;	//Длина поля данных в байтах
        txf.can_id = 1212;  //0x05e5 функция включения адресс
        txf.data[0] = static_cast<int8_t>(-1); //коордианата x изменяется на -1
        txf.data[1] = static_cast<int8_t>(0); //координата y постоянна
        txf.data[2] = 0x00;
        txf.data[3] = 0x80;
        server -> FramesToBytes(&txf);
    }
}

void TrackingZala::normalMode()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] = 0b00000010;
    txf.data[1] = 0b00000000;
    txf.data[2] = 0b00010010;
    txf.data[3] = 0b00010001;
    server -> FramesToBytes(&txf);
}

void TrackingZala::digitalStabilization()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00001100;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00001001;
    txf.data[3] =0b00010001;
    server -> FramesToBytes(&txf);
}
void TrackingZala::digitalStabilizationOFF()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00000000;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00001001;
    txf.data[3] =0b00010001;
    server -> FramesToBytes(&txf);
}

void TrackingZala::pictureInPicture()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00000110;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00000000;
    txf.data[3] =0b00000000;
    server -> FramesToBytes(&txf);
}

void TrackingZala::smallPictureInPicture()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00001111;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00000000;
    txf.data[3] =0b00000000;
    server -> FramesToBytes(&txf);
}

void TrackingZala::horizontalLine()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00001101;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00000000;
    txf.data[3] =0b00000000;
    server -> FramesToBytes(&txf);
}

void TrackingZala::verticalLine()
{
    txf.can_dlc = 4;
    txf.can_id = 1211;
    txf.data[0] =0b00001110;
    txf.data[1] =0b00000000;
    txf.data[2] =0b00000000;
    txf.data[3] =0b00000000;
    server -> FramesToBytes(&txf);
}

void TrackingZala::inversiaModeAc(bool pFlag)
{
    if (pFlag)
    {
        txf.can_dlc = 4;
        txf.can_id = 1211;
        txf.data[0] =0b00001000;
        txf.data[1] =0b00000000;
        txf.data[2] =0b00000000;
        txf.data[3] =0b00000000;
        server -> FramesToBytes(&txf);
    }
    else
    {
        txf.can_dlc = 4;
        txf.can_id = 1211;
        txf.data[0] =0b00000111;
        txf.data[1] =0b00000000;
        txf.data[2] =0b00000000;
        txf.data[3] =0b00000000;
        server -> FramesToBytes(&txf);
    }
}

float TrackingZala::angleAir(double spX, double spY, double spR)
{
    float alpha = 0.0f;

//  0...90 angle
    if ((spX > 0) and (spY > 0))
    {
        alpha = acos((spR*spR + spX*spX - spY*spY)/(2*spR*spX));
        alpha = alpha * (180/acos((long double) -1));
    }
    else

//      90...180 angle
        if ((spX < 0) and (spY > 0))
        {
            spX = spX * (-1);
            alpha = acos((spR*spR+spX*spX-spY*spY)/(2*spR*spX));
            alpha = alpha * (180/acos((long double) -1));
            alpha = 180 - alpha;
        }
        else

//          180...270 angl7e
            if ((spX < 0) and (spY < 0))
            {
                spX = spX * (-1);
                spY = spY * (-1);
                alpha = acos((spR*spR + spX*spX - spY*spY)/(2*spR*spX)); //0...90
                alpha = alpha * (180/acos((long double) - 1));
                alpha = 180 + alpha;
            }
            else

//              270...360 angle
                if ((spX > 0) and (spY < 0))
                {
                    spY = spY * (-1);
                    alpha = acos((spR*spR + spX*spX - spY*spY)/(2*spR*spX)); //0...90
                    alpha = alpha * (180/acos((long double) - 1));
                    alpha = 360 - alpha;
                }
    return alpha;
}

/*Функция в которой включается задержка по ней выполняется реакция на принудительное движение платформы*/
void TrackingZala::ac_Joystick()
{
    if ((mControllerManager -> lPosJoyX !=0) or (mControllerManager -> lPosJoyY !=0))
    {
        if (!(mTimer -> isActive()))
        {
            mTimer -> start(245);
            connect(mTimer, SIGNAL(timeout()), SLOT(AC_Joystick())); // по завершению отчета таймера выполнить слот на получение пакетов 900
            f_send900 = true;
        }
    }
    else
    {
        if (mTimer -> isActive())
        {
            mTimer -> stop();
            f_on_900 = false;
            f_send900 = false;
        }
    }  
}
void TrackingZala::queryRange_ac()
{
    if (mControllerManager->mPermission)
    {
        emit newLogMsg("+++ Range");
        mControllerManager->queryRange_ac();
    }
    emit newLogMsg("new Range");

}

/*Реакция на принудительное движение платформы, пакеты 900*/
void TrackingZala::AC_Joystick()
{
    if (f_send900)
    {
        can_frame txf2;
        txf2.can_dlc = 4;
        txf2.can_id = 900;
        txf2.data[0] = 0b00000001;
        txf2.data[1] = 0b00000001;
        txf2.data[2] = 0b00000001;
        txf2.data[3] = 0b00000001;
        server -> FramesToBytes(&txf2);
        mTimer->stop();
        if (!f_on_900)
        {
            readVPx (0.0f);
            readVPy (0.0f);
            mPIDx -> pid_on_off(false, 0.0f);
            mPIDy -> pid_on_off(false, 0.0f);
            f_on_900 = true;
        }
	f_send900 = false;
    }
}

void TrackingZala::readVPx(float pv)
{
    mControllerManager -> pidX = static_cast<long>(pv);
    spMotorX = static_cast<double>(pv);

    if (pv != 0.0f)
    {
        if (VsrX[0] != 0)
       {
            memmove (VsrX+1,                        /*kyga*/
                     VsrX,                      	/*otkyga*/
                     sizeof(VsrX)-sizeof(VsrX[0]));	/*ckoJlbko bytes*/
        VsrX[0] = pv;
        }
        else for (int i=0; i<100; i++) VsrX[i]=pv;
        for (int i=10; i<35; i++) SrAX += VsrX[i];
        SrAX /= 25.0f;
        }
    else for (int i=0; i<100; i++) VsrX[i]=pv;
    mControllerManager->spVedX = static_cast<long>(SrAX);
}

void TrackingZala::readVPy(float pv)
{
    mControllerManager -> pidY = static_cast<long>(pv);
    spMotorY = static_cast<double>(pv);
    
    if (pv != 0.0f)
    {
        if (VsrY[0] != 0)
        {
            memmove (VsrY+1,                        /*kyga*/
                     VsrY,                          /*otkyga*/
                     sizeof(VsrY)-sizeof(VsrY[0])); /*ckoJlbko*/
        VsrY[0] = pv;
        }
        else for (int i=0; i<100; i++) VsrY[i]=pv;
        for (int i=10; i<35; i++) SrAY += VsrY[i];
        SrAY /= 25.0f;
    }
    else for (int i=0; i<100; i++) VsrX[i]=pv;
    mControllerManager->spVedY = static_cast<long>(SrAY);
}

void TrackingZala::acRamkaFreez(bool move)
{
    if (move)
    {
        f_send900 = true;
        f_on_900  = false;
        AC_Joystick();
    }
    else
    {
        f_send900 = true;
        f_on_900  = false;
        AC_Joystick();
    }
}

/*Функция на получение координат от платы*/
void TrackingZala::getKoordinatAc(can_frame *frame)
{
        if (!mControllerManager -> enabled_as)
        {
            can_frame txf1;
            txf1 = *frame;
            float x1 = 0.0f;
            float y1 = 0.0f;
            if (txf1.can_id == 1210)
            {
                filtrTimer -> start(200); //запуск таймера

                xZala = (static_cast<int16_t>(txf1.data[1]));
                xZala <<= 8;
                xZala |= (static_cast<int16_t>(txf1.data[0]));

                yZala = (static_cast<int16_t>(txf1.data[3]));
                yZala <<= 8;
                yZala |= (static_cast<int16_t>(txf1.data[2]));

                indikator_ac = true;
                flag_konstr  = false;
                x1 = static_cast<float>(xZala);
                y1 = static_cast<float>(yZala);
                if (mPID -> page_pid)
                {
                    connect (mPID, SIGNAL (readPidX (float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                             mPIDx,  SLOT (writePidX(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)));
                    connect (mPID,  SIGNAL(readPidY (float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)),
                             mPIDy,   SLOT(writePidY(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float)));
                    mPID -> page_pid = false;

                }
            }

            if (txf1.can_id != 1210) connect(filtrTimer, SIGNAL(timeout()),this,SLOT(filterIndikator()));
            if ((!t_indi_ac) and (indikator_ac)) connect (timerD, SIGNAL(timeout()), this, SLOT(time_indi_ac()));
            if (( t_indi_ac) and (indikator_ac))
            {
                //Диапазон X:[-252..+252](84)   Y:[-208..+208](70)
                if (!ramka) //ramka bolshaya
                {
                    if ((xZala <= gb0) and (xZala >= -gb0) and (yZala <= gb0) and (yZala >= -gb0))
                    {
                        mPIDx -> pid.Zona_kff = 0;
                        mPIDy -> pid.Zona_kff = 0;
                    }//zona vnutri ramki
                    else
                    {
                        if((xZala >= -gbx1) and (xZala <= gbx1) and (yZala >= -gby1) and (yZala <= gby1))
                        {
                            mPIDx -> pid.Zona_kff = 1;
                            mPIDy -> pid.Zona_kff = 1;
                        }//zona okrest ramki
                        else
                        {
                            if((xZala < -gbx1)or(xZala > gbx1)or(yZala < -gby1)or(yZala > gby1))
                            {
                                mPIDx -> pid.Zona_kff = 2;
                                mPIDy -> pid.Zona_kff = 2;
                            }//zona dalnyaya
                        }
                    }
                }
                else        //ramka malaya
                {
                    if ((xZala <= gm0) and (xZala >= -gm0) and (yZala <= gm0) and (yZala >= -gm0))
                    {
                        mPIDx -> pid.Zona_kff = 0;
                        mPIDy -> pid.Zona_kff = 0;
                    }//zona vnutri ramki
                    else
                    {
                        if((xZala >= -gmx1) and (xZala <= gmx1) and (yZala >= -gmy1) and (yZala <= gmy1))
                        {
                            mPIDx -> pid.Zona_kff = 1;
                            mPIDy -> pid.Zona_kff = 1;
                        }//zona okrest ramki
                        else
                        {
                            if((xZala < -gmx1)or(xZala > gmx1)or(yZala < -gmy1)or (yZala > gmy1))
                            {
                                mPIDx -> pid.Zona_kff = 2;
                                mPIDy -> pid.Zona_kff = 2;
                            }//zona dalnyaya
                        }
                    }
                }

                mPIDx -> pid_on_off(indikator_ac, x1);
                connect(mPIDx, SIGNAL(readValuePid(float)), this, SLOT(readVPx(float)));
                mPIDy -> pid_on_off(indikator_ac, y1);
                connect(mPIDy, SIGNAL(readValuePid(float)), this, SLOT(readVPy(float)));
                spRezult = hypot(spMotorX,spMotorY);
                spAngle = angleAir(spMotorX,spMotorY,spRezult);
                t_indi_ac = false;
                timerD -> start(static_cast<int>(mPIDx -> pid.Tmls));
            }
        }
}
/*Функция на фильтрацию пакетов*/
void TrackingZala::filterIndikator()
{
    if (!flag_konstr)
    {
        flag_konstr  = true;
        indikator_ac = false;
        mPIDx -> pid_on_off(indikator_ac, 0);
        mPIDy -> pid_on_off(indikator_ac, 0);
        connect(mPIDx, SIGNAL(readValuePid(float)), this, SLOT(readVPx(float)));
        connect(mPIDy, SIGNAL(readValuePid(float)), this, SLOT(readVPy(float)));
    }
}

void TrackingZala::time_indi_ac()
{
    t_indi_ac = true;
}



/*установка параметров типа цели для сопровождения*/
void TrackingZala::setAimType(AimType pAimtype)
{
    mOptWidget->setAimType(pAimtype);
    switch (pAimtype)
    {
    case HOME:
        ramka = false;
        txf.can_dlc = 4;
        txf.can_id = 1211;
        txf.data[0] = 0b00011110;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00000000;
        txf.data[3] = 0b00000000; //присвоили значение 00011110
    break;

    case CAR:
        ramka = true;
        txf.can_dlc = 4;
        txf.can_id = 1211;
        txf.data[0] = 0b00011111;
        txf.data[1] = 0b00000000;
        txf.data[2] = 0b00000000;
        txf.data[3] = 0b00000000;
    default:
        break;
    }

    server->FramesToBytes(&txf);
}

/*Функция перемещения рамки целеуказателя, абсолютное перемещение*/
void TrackingZala::move_Ac_Target(long x, long y)
{
    //QString prov;
    txf.can_dlc = 4;	//Длина поля данных в байтах
    txf.can_id = 1212;  //0x05e5 функция включения адресс
    txf.data[0] = static_cast<int8_t>(x); //коордианата x
    txf.data[1] = static_cast<int8_t>(y); //координата y
    txf.data[2] = 0x00;
    txf.data[3] = 0x81;
    server -> FramesToBytes(&txf);
}



