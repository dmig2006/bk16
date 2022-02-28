#include "mainwindow.h"
#include "ui_mainwindow.h"

 /*
 menu_PID меню для настйроки PID регулятора удалено
*/

/*Конструктор класса11*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette = ui->powerSupplyInfo->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    QIntValidator *range_validator = new QIntValidator(0, 2600, this);
    QIntValidator *pressure_validator = new QIntValidator(400, 800, this);
    QIntValidator *temperature_validator = new QIntValidator(-45, 70, this);
    QIntValidator *wind_validator = new QIntValidator(-20, 20, this);
    ui->rangeInput->setValidator(range_validator);
    ui->pressureInput->setValidator(pressure_validator);
    ui->temperatureInput->setValidator(temperature_validator);
    ui->windXInput->setValidator(wind_validator);
    ui->windYInput->setValidator(wind_validator);

    mControllerManager = new ControllerManager();

    mVideoWidget = new VideoWidget(this);
    mVideoWidget->setGeometry(10, 35, 768, 576);

    mOptWidget = new OptWidget(this);
    mOptWidget->setGeometry(10, 35, 768, 576);
    mOptWidget->setControllerManager(mControllerManager);

    mPlathormDir = new PlathormDir(this);
    mPlathormDir->setGeometry(615, 640, 180, 110);

    mSoundSignalDialog = new SoundSignalDialog(this);
    mSoundSignalDialog->setControllerManager(mControllerManager);
    mSoundSignalDialog->hide();

    mBeeper = new Beeper(this);

    ui -> actionKORD -> setChecked(true);

    mTypeTracking = 0;
    mLogId = 1;
    isVideo = false;
    isFilter = false;

    isOptRange = false;
    mLogID = 0;
    isTracking = false;
    mPID   = new PID;
    mPID -> page_pid = false;
    mPID   -> setGeometry(820,0,180,690);
//ZALA
  	mTrackingZala = new TrackingZala();
   	mTrackingZala -> setControllerManager(mControllerManager);
	mTrackingZala -> setOptWidget(mOptWidget);
	mTrackingZala -> setPID(mPID);
	mTrackingZala -> setVideoWidget(mVideoWidget);
	connect(mTrackingZala, SIGNAL(newLogMsg(QString)),SLOT(appendLog(QString)));
	test = true;
//ZALA

    inversia_stabilization = true;	
    inversia_picture = false;
    invers = false;

    qRegisterMetaType <CameraMode> ("CameraMode");
    qRegisterMetaType <BurstType> ("BurstType");
    qRegisterMetaType <FireType> ("FireType");
    qRegisterMetaType <Mode> ("Mode");
    qRegisterMetaType <Cocking> ("Cocking");
    qRegisterMetaType <AimType> ("AimType");

    connect(mControllerManager, SIGNAL(newRange(int)),                   SLOT(setRange(int)));
    connect(mControllerManager, SIGNAL(newJust(long,long)),              SLOT(setJustCross(long,long)));
    connect(mControllerManager, SIGNAL(newCorrectionCross(long,long)),   SLOT(setCorrectionCross(long,long)));
    connect(mControllerManager, SIGNAL(newSpeedX(float)),                SLOT(setSpeedX(float)));
    connect(mControllerManager, SIGNAL(newSpeedY(float)),                SLOT(setSpeedY(float)));
    connect(mControllerManager, SIGNAL(newPositionX(long)),              SLOT(setPositionX(long)));
    connect(mControllerManager, SIGNAL(newPositionY(long)),              SLOT(setPositionY(long)));
    connect(mControllerManager, SIGNAL(newVoltageBu(float)),             SLOT(setVoltageBu(float)));
    connect(mControllerManager, SIGNAL(newThermo(int)),                  SLOT(setThermo(int)));
    connect(mControllerManager, SIGNAL(newPressure(int,bool)),           SLOT(setPressure(int)));
    connect(mControllerManager, SIGNAL(newVoltageExternal(float)),       SLOT(setVoltage(float)));
    connect(mControllerManager, SIGNAL(newPermission(bool)),             SLOT(setPermission(bool)));
    connect(mControllerManager, SIGNAL(newAmmunitionCounter(int)),       SLOT(setAmmunitionCounter(int)));
    connect(mControllerManager, SIGNAL(newFire(bool)),                   SLOT(setFire(bool)));
    connect(mControllerManager, SIGNAL(newBurstType(BurstType)),         SLOT(setBurstType(BurstType)));
    connect(mControllerManager, SIGNAL(newFireType(FireType)),           SLOT(setFireType(FireType)));
    connect(mControllerManager, SIGNAL(newStabilization(bool)),          SLOT(setStabilization(bool)));
    connect(mControllerManager, SIGNAL(newLastSaved(long,long,int,int)), SLOT(setLastSaved(long,long,int,int)));
    connect(mControllerManager, SIGNAL(newAutoMode(bool)),               SLOT(setAutoMode(bool)));
    connect(mControllerManager, SIGNAL(newMode(Mode)),                   SLOT(setMode(Mode)));
    connect(mControllerManager, SIGNAL(newLogMsg(QString)),              SLOT(appendLog(QString)));

    //Выбор рамки
    connect(mControllerManager, SIGNAL(setCamera_AC(CameraMode)),        SLOT(setCameraMode_AC(CameraMode)));
    //ZAla
    //Включение автосоправождение
    connect(mControllerManager, SIGNAL (newTrack(bool)),                 SLOT (acTrack(bool)));
    //блокировка поворотной платформы
    connect(mControllerManager, SIGNAL(newTrack(bool)),mOptWidget,       SLOT(setBlockPP(bool)));
    //абсолютное перемещение рамки
    //ZALA
    connect(mControllerManager, SIGNAL(newClose()),                      SLOT(closeAll()));
    connect(mControllerManager, SIGNAL(newCocking(Cocking)),             SLOT(setCocking(Cocking)));
    connect(mControllerManager, SIGNAL(newReset()),                      SLOT(resetBeep()));
    connect(mControllerManager, SIGNAL(newAimType(AimType)),mTrackingZala, SLOT(setAimType(AimType)));
	connect(mControllerManager, SIGNAL(setConnectedPlathorm(bool)),      SLOT(setActivatePlathorm(bool)));
    connect(mControllerManager, SIGNAL(invertCrossColor()), mOptWidget, SLOT(invertCrossColor()));

    connect(mVideoWidget, SIGNAL(newLogMsg(QString)), SLOT(appendLog(QString)));

    connect(mBeeper, SIGNAL(newLogMsg(QString)), SLOT(appendLog(QString)));

    connect(mSoundSignalDialog, SIGNAL(testPlay(BeepState)),     SLOT(testBeep(BeepState)));
    connect(mSoundSignalDialog, SIGNAL(newBeepState(BeepState)), SLOT(setBeepState(BeepState)));
    connect(mSoundSignalDialog, SIGNAL(closed()),                SLOT(enableInterface()));

    connect(ui->actionSound_signal, SIGNAL(triggered()), SLOT(showSoundSignalDialog()));

    connect(ui->actionAdjustment,   SIGNAL(triggered()), SLOT(showAdjustmentDialog()));

    connect(mVideoWidget, SIGNAL(resetCamera()), SLOT(resetCamera()));

    connect(mControllerManager, SIGNAL(closePID()), mPID, SLOT(closePid()));

    setFireType(SINGLE_FIRE);
    setBurstType(SHORT_BURST);
    setMode(MODE_SEMI_AUTO);
    setStabilization(false);
    setFire(false);
    setCameraMode(WIDE_FORMAT);

    mTrackingZala -> setCameraModeAC(WIDE_FORMAT);

    setPermission(false);
    mControllerManager->start();
    appendLog(tr("Application is runing"));
    is_ctrl_pressed = false;
    is_s_pressed = false;
    is_t_pressed = false;
    isBeepIpa = false;

    is_low_cu_voltage = false;
    is_low_rp_voltage = false;
    is_inverted_enable = false;

    is_first_beep = true;

    is_wiper_working = false;
    cur_weapon = WEAPON_KORD;

    flagActivate = false;
    flagInversia = true;
    flagPid = false;

    is_p = false;
    is_i = false;
    is_d = false;

    setActivatePlathorm(false);

    ui -> centralWidget -> setFocus();

    paintNormal();
    mTrackingZala -> setCameraModeAC(WIDE_FORMAT);
    ui->onOffGor->setChecked(true);
    mControllerManager->f_uprGor = ui->onOffGor->isChecked();
    mControllerManager->f_uprVer = ui->onOffVer->isChecked();


}

/*Деструктор класса*/
MainWindow::~MainWindow()
{
    mTrackingZala->acTrack(false);
    delete mBeeper;
    delete mSoundSignalDialog;
    delete mPlathormDir;
    delete mOptWidget;
    delete mVideoWidget;
    delete mControllerManager;
    delete mPID;
	delete mTrackingZala;	

    delete ui;
}

/*Включение режима сопровождения*/
void MainWindow::acTrack(bool enabled)
{
    if (enabled) //Проверка триггера на включение/отключение сопровождения
    {
        ui -> tracking -> setText("ВКЛ");
        ui -> centralWidget->setFocus();
        mTrackingZala ->acTrack(true);
    }
    else
    {
        ui -> tracking -> setText("ВЫКЛ");
		mTrackingZala -> acTrack(false);
    }
}

/*Выбор режимов камеры*/
void MainWindow::setCameraMode_AC(CameraMode pCameraMode)
{
    switch (pCameraMode)
    {
        case WIDE_FORMAT:
            ui->camera->setText(tr("WFOV"));
            mTrackingZala -> setCameraModeAC(WIDE_FORMAT);
           break;
        case NARROW_FORMAT:
            //acTrack(false);
            ui->camera->setText(tr("NFOV"));
            mTrackingZala -> setCameraModeAC(NARROW_FORMAT);
            break;
        case THERMAL:

            ui->camera->setText(tr("IR"));
            mTrackingZala->setCameraModeAC(THERMAL);
            break;
        case ZOOM_THERMAL:
            ui->camera->setText(tr("IR(X2)"));
            mTrackingZala->setCameraModeAC(ZOOM_THERMAL);
            break;
     }
}

/*закрытие основного окна*/
void MainWindow::closeAll()
{
    mVideoWidget->close();
    mOptWidget->close();
    close();
}

/*обработка отпусканий клавиш*/
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() != Qt::ControlModifier)
        is_ctrl_pressed = false;
    switch (event->key()) {
    case Qt::Key_S:
        is_s_pressed = false;
        break;
    case Qt::Key_T:
        is_t_pressed = false;
        break;
    case Qt::Key_P:
        is_p = false;
        break;
    case Qt::Key_I:
        is_i = false;
        break;
    case Qt::Key_D:
        is_d = false;
        break;
    default:
        QWidget::keyReleaseEvent(event);
    }
}

/*обработка нажатий клавиш*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        is_ctrl_pressed = true;
        if (is_s_pressed && is_t_pressed)
        {
            flagPid = true;
            mPID->close();
            closeAll();
        }


    } else {
        is_ctrl_pressed = false;
    }
    switch (event->key()) {
    case Qt::Key_S:
        is_s_pressed = true;
        resetBeep();
        if (is_ctrl_pressed && is_t_pressed)
        {

            flagPid = true;

            mPID->close();

            closeAll();
        }
        break;
    case Qt::Key_T:
        is_t_pressed = true;
        resetBeep();
        if (is_ctrl_pressed && is_s_pressed)
        {
            flagPid = true;
            mPID->close();
            closeAll();
        }
        break;
    case Qt::Key_Escape:
        resetBeep();
        mControllerManager->closeAutoMode();
        break;
    case Qt::Key_F1:
        resetBeep();
        if (mOptWidget) {
            if (!isOptRange) {
                mOptWidget->setVisibleOptRange(true);
                ui->rangeInput->setEnabled(false);
                isOptRange= true;
            } else {
                mOptWidget->setVisibleOptRange(false);
                ui->rangeInput->setEnabled(true);
                isOptRange= false;
            }
        }
        break;
    case Qt::Key_F2:
        resetBeep();
        if (mOptWidget && isOptRange)
            mOptWidget->optRangeUp();
        break;
    case Qt::Key_F3:
        resetBeep();
        if (mOptWidget && isOptRange)
            mOptWidget->optRangeDown();
        break;
    case Qt::Key_F4:
        resetBeep();
        if (!mOptWidget->getFilter()) {
            mOptWidget->setFilter(true);
            appendLog(tr("Filter enabled"));
        } else {
            mOptWidget->setFilter(false);
            appendLog(tr("Filter disabled"));
        }
        break;
    case Qt::Key_F5:
        mOptWidget->setPixmapOptEnabled();
        break;

    case Qt::Key_F6:
        is_wiper_working = !is_wiper_working;
        mControllerManager->switchWiper(is_wiper_working);
        ui->wiper->setText(is_wiper_working ? tr("ON") : tr("OFF"));
        break;
        //Inversia pallete
            case Qt::Key_F7:
                if (flagInversia)
                {
                    flagInversia = false;
                    inversia(false);
                }
                else
                {
                    flagInversia = true;
                    inversia(true);
                }
                break;
        //Inversia pallete

    case Qt::Key_F8:
        if(!mControllerManager->getBlocking()){
            if(mControllerManager->getMode() == MODE_SEMI_AUTO && !mControllerManager->isAddFireProcess())
                mControllerManager->setMode(MODE_MARCH);
        }
        break;

    case Qt::Key_F9:
    {
        mControllerManager->switchLight();
        break;
    }

//ZALA_CLASS
             //Picture mode
             case Qt::Key_F10:
                static char videoType;
                videoType++;
                inversia_stabilization = false;
                if (videoType > 4) videoType = 0;
                switch (videoType)
                {
                    case 0:
                        mTrackingZala -> normalMode();
                    break;

                    case 1:
                        mTrackingZala -> pictureInPicture();
                    break;

                    case 2:
                        mTrackingZala -> smallPictureInPicture();
                    break;

                    case 3:
                        mTrackingZala -> horizontalLine();
                    break;

                    case 4:
                        mTrackingZala -> verticalLine();
                    break;
                }
                break;

                //Digital Stabilization
            case Qt::Key_F11:
                if(!inversia_stabilization)
                {
                    mTrackingZala -> digitalStabilization();
                    inversia_stabilization = true;
                break;
                }
                else
                {
                    mTrackingZala -> digitalStabilizationOFF();
                    inversia_stabilization = false;
                break;
                }
            case Qt::Key_F12:
                if(inversia_picture)
                {
                    mTrackingZala ->inversiaModeAc(true);
                    inversia_picture = false;
                    break;
                }
                else
                {
                    mTrackingZala -> inversiaModeAc(false);
                    inversia_picture = true;
                    break;
                }

                //Move centr display
            case Qt::Key_Up:
                if (!(mControllerManager -> enabled_as))
                {
                    mTrackingZala -> upMove();
                }
                break;

            case Qt::Key_Down:
                if (!(mControllerManager -> enabled_as))
                {
                    mTrackingZala -> downMove();
                }
                break;

            case Qt::Key_Right:
                if (!(mControllerManager -> enabled_as))
                {
                    mTrackingZala -> rightMove();
                }
                break;

            case Qt::Key_Left:
                if (!(mControllerManager -> enabled_as))
                {
                    mTrackingZala -> leftMove();
                }
            break;

        case Qt::Key_P:
            is_p = true;
            if (is_i && is_d) 
            {
                mPID -> show();
                is_p = false;
            }

        break;

        case Qt::Key_I:
            is_i = true;
            if (is_p && is_d)
            {
                mPID->show();
                is_i = false;
            }
        break;

    case Qt::Key_D:
            is_d = true;
            if (is_p && is_i)
            {
                mPID->show();
                is_d = false;
            }
        break;
    }
}

/*добавление строки в лог сообщений*/
void MainWindow::appendLog(QString pStr)
{
    ui -> LogWidget -> setEnabled(false);

    QListWidgetItem *newItem = new QListWidgetItem;
    QString num;
    num.setNum(mLogId);
    newItem->setText(num + QString("> ") + pStr);
    ui->LogWidget->insertItem(1, newItem);
    mLogId++;
}

/*действия при взводе вооружения*/
void MainWindow::setCocking(Cocking pCocking)
{
    switch (pCocking) {
    case COCKING_LOAD:
        setMessage(ui->cocking, tr("LOADING"));
        setInterfaceInteractivity(false);
        mOptWidget->setCocking(true);
        break;
    case COCKING_COMPLETED:
        setMessage(ui->cocking, tr("COMPLETED"));
        setInterfaceInteractivity(true);
        mOptWidget->setCocking(false);
        break;
    case COCKING_ERROR:
        setMessage(ui->cocking, tr("ERROR"), unsafe);
        setInterfaceInteractivity(true);
        mOptWidget->setCocking(false);
        break;
    }
}

/*включение элементов окна при наличии сохранённых целей*/
void MainWindow::setAutoMode(bool state)
{
    if (AimList::getSingletonPtr()->size() > 0) {
        state=state;
    }
}

/*отображение скорости по горизонтали*/
void MainWindow::setSpeedX(float pSpeed)
{
    if (pSpeed >= 59.98)
        pSpeed = 60.0;
    if (pSpeed <= -59.98)
        pSpeed = -60.0;
    ui->horizontalSpeed->setText(QString::number(pSpeed, 'f', 2));
}

/*отображение скорости по вертикали*/
void MainWindow::setSpeedY(float pSpeed)
{
    if (pSpeed >= 59.98)
        pSpeed = 60.0;
    if (pSpeed <= -59.98)
        pSpeed = -60.0;
    ui->verticalSpeed->setText(QString::number(pSpeed, 'f', 2));
}

/*отображение позиции по горизонтали*/
void MainWindow::setPositionX(long pPosition)
{
    mOptWidget->setPositionEncoderX(pPosition);
    float angle = 0.0f;
    angle = static_cast<float>(pPosition) / DEGREE_TO_CENTIDEGREE
            - (((int)(static_cast<float>(pPosition) / DEGREE_TO_CENTIDEGREE) / 360) * 360);
    if (angle < 0.0f)
        angle += 360.0f;
    mPlathormDir->setAngle(angle);
    ui->horizontalAngle->setText(QString::number(angle, 'f', 2) + QString(QChar(0x00b0)));
}

/*отображение позиции по вертикали*/
void MainWindow::setPositionY(long pPosition)
{
    mOptWidget->setPositionEncoderY(pPosition);
    ui->verticalAngleSlider->setValue(pPosition);
    if (pPosition >= 6998)
        pPosition = 7000;
    if (pPosition <= -1998)
        pPosition = -2000;
    float angle = static_cast<double>(pPosition) / DEGREE_TO_CENTIDEGREE;
    ui->verticalAngle->setText(QString::number(angle, 'f', 2)                               + QString(QChar(0x00b0)));
}

/*отображение напряжения питания БУ*/
void MainWindow::setVoltageBu(float pVoltageExternal)
{
    if (pVoltageExternal < 22.0) {
        setMessage(ui->controlUnitVoltage,
                   QString::number(pVoltageExternal, 'f', 1),
                   unsafe);
        appendLog(tr("WARNING!! LOW CU VOLTAGE"));
        setMessage(ui->powerSupplyInfo, tr("WARNING!! LOW VOLTAGE"), unsafe);
        is_low_cu_voltage = true;
        mBeeper->setBatteryBeepFlag(true, is_first_beep);
        is_first_beep = false;
    } else if (!is_low_rp_voltage) {
        setMessage(ui->controlUnitVoltage,
                   QString::number(pVoltageExternal, 'f', 1),
                   neutral);
	is_low_cu_voltage = false;
        ui->powerSupplyInfo->clear();
        mBeeper->setBatteryBeepFlag(false, false);
        is_first_beep = true;
    } else if (is_low_rp_voltage) {
        is_first_beep = false;
        is_low_cu_voltage = false;
        setMessage(ui->controlUnitVoltage,
                   QString::number(pVoltageExternal, 'f', 1),
                   neutral);
    }
}

/*отбражение температуры*/
void MainWindow::setThermo(int pThermo)
{
    ui->temperatureInput->setText(QString::number(pThermo));
}

/*отбражение давления*/
void MainWindow::setPressure(int pPressure)
{
    ui->pressureInput->setText(QString::number(pPressure));
}

/*отображение напряжения питания ПП*/
void MainWindow::setVoltage(float pVoltage)
{
    if (pVoltage < 22.0) {
        setMessage(ui->rotatingPlatformVoltage,
                   QString::number(pVoltage, 'f', 1),
                   unsafe);
        appendLog(tr("WARNING!! LOW RP VOLTAGE"));
        setMessage(ui->powerSupplyInfo, tr("WARNING!! LOW VOLTAGE"), unsafe);
        is_low_rp_voltage = true;
        mBeeper->setBatteryBeepFlag(true, is_first_beep);
        is_first_beep = false;
    } else if (!is_low_cu_voltage) {
        setMessage(ui->rotatingPlatformVoltage,
                   QString::number(pVoltage, 'f', 1),
                   neutral);
        is_low_rp_voltage = false;
        ui->powerSupplyInfo->clear();
        mBeeper->setBatteryBeepFlag(false, false);
        is_first_beep = true;
    } else if (is_low_cu_voltage) {
        is_first_beep = false;
        is_low_rp_voltage = false;
        setMessage(ui->rotatingPlatformVoltage,
                   QString::number(pVoltage, 'f', 1),
                   neutral);
    }
}

/*отображение дальности*/
void MainWindow::setRange(int pRange)
{
    ui->rangeInput->setText(QString::number(pRange));
    mOptWidget->setRange(pRange);
}

/*отображение счётчика патронов*/
void MainWindow::setAmmunitionCounter(int pAmmunitionCounter)
{
    if (pAmmunitionCounter >= 0)
        ui->ammunition->setText(QString::number(pAmmunitionCounter));
}

/*отображение состояния разрешения огня*/
void MainWindow::setPermission(bool state)
{
    if (state)
        setMessage(ui->fireResolution, tr("ARM"), unsafe);
    else
        setMessage(ui->fireResolution, tr("SAFE"), safe);
}

/*отображение длины очереди*/
void MainWindow::setBurstType(BurstType pBurstType)
{
    if (mControllerManager->getFireType() == TURN_FIRE)
        switch (pBurstType) {
        case SHORT_BURST:
            ui->burstType->setText(tr("SHORT"));
            break;
        case LONG_BURST:
            ui->burstType->setText(tr("LONG"));
            break;
        }
}

/*отображение типа стрельбы*/
void MainWindow::setFireType(FireType pFireType)
{
    switch (pFireType) {
    case SINGLE_FIRE:
        ui->fireType->setText(tr("CONT."));
        ui->burstType->setText("---");
        break;
    case TURN_FIRE:
        ui->fireType->setText(tr("BURST"));
        BurstType pBurstType = mControllerManager->getBurstType();
        switch (pBurstType) {
        case SHORT_BURST:
            ui->burstType->setText(tr("SHORT"));
            break;
        case LONG_BURST:
            ui->burstType->setText(tr("LONG"));
            break;
        }
        break;
    }
}

/*установка координат перекрестия баллистики*/
void MainWindow::setCorrectionCross(long pCorrectionX, long pCorrectionY)
{
    if (mOptWidget)
        mOptWidget->setCorrectionCross(pCorrectionX, pCorrectionY);
}

/*установка координат основного перекрестия*/
void MainWindow::setJustCross(long pJutsX, long pJutsY)
{
    if (mOptWidget)
        mOptWidget->setJustCross(pJutsX, pJutsY);
}

/*отображение состояния стабилизации*/
void MainWindow::setStabilization(bool state)
{
    if (state)
        ui->stabilization->setText(tr("ON"));
    else
        ui->stabilization->setText(tr("OFF"));
    mOptWidget->setStabilization(state);
}

/*отображение стосояния огня*/
void MainWindow::setFire(bool state)
{
    if (state)
    {
        setMessage(ui->fire, tr("YES"), unsafe);

        mControllerManager->f_uprGor = ui->onOffGor->isChecked();
        mControllerManager->f_uprVer = ui->onOffVer->isChecked();

        double range = mControllerManager->winRange;
        double vsrx  = mControllerManager->VsrXX;   if (vsrx  < 0) vsrx  *= -1;
        double vsry  = mControllerManager->VsrYY;   if (vsry  < 0) vsry  *= -1;
        double uprAx = mControllerManager->uprAngX; if (uprAx < 0) uprAx *= -1;
        double uprAy = mControllerManager->uprAngY; if (uprAy < 0) uprAy *= -1;
        double uprMx = mControllerManager->uprMetX; if (uprMx < 0) uprMx *= -1;
        double uprMy = mControllerManager->uprMetY; if (uprMy < 0) uprMy *= -1;

        ui->gorSpKm  ->setText(QString::number((double)(vsrx*range*60.0*2*M_PI*3.6/4250/360),'g',2));
        ui->gorSpMs  ->setText(QString::number((double)(vsrx*range*60.0*2*M_PI*1.0/4250/360),'g',2));
        ui->gorSpNods->setText(QString::number((double)(vsrx*range*60.0*2*M_PI*3.6/1.852/4250/360),'g',2));
        ui->gorAngle ->setText(QString::number((double)(uprAx/100.0),'g',2));
        ui->gorMeters->setText(QString::number((double)(uprMx),'g',2));

        ui->verSpKm  ->setText(QString::number((double)(vsry*range*60.0*2*M_PI*3.6/4250/360),'g',2));
        ui->verSpMs  ->setText(QString::number((double)(vsry*range*60.0*2*M_PI*1.0/4250/360),'g',2));
        ui->verSpNods->setText(QString::number((double)(vsry*range*60.0*2*M_PI*3.6/1.852/4250/360),'g',2));
        ui->verAngel ->setText(QString::number((double)(uprAy/100.0),'g',2));
        ui->verMeters->setText(QString::number((double)(uprMy),'g',2));

        mControllerManager->koffupr = (double)ui->sliderUpr->value() / 100.0;
        ui->koffUpr->setText(QString::number(mControllerManager->koffupr));

        mControllerManager->VsrX[0] = 0;
        mControllerManager->VsrY[0] = 0;
        mTrackingZala->VsrX[0] = 0;
        mTrackingZala->VsrY[0] = 0;
    }
    else
        setMessage(ui->fire, tr("NO"), safe);
}

void MainWindow::on_sliderUpr_actionTriggered(int action)
{
    action = action;
}
void MainWindow::on_sliderUpr_sliderMoved(int position)
{
    mControllerManager->koffupr = position / 100.0;
    ui->koffUpr->setText(QString::number(mControllerManager->koffupr));
}
void MainWindow::on_koffLeft_clicked()
{
    double x = (double)ui->sliderUpr->value();
    x--;
    ui->sliderUpr->setValue(x);
    ui->sliderUpr->update();
    mControllerManager->koffupr = (double)ui->sliderUpr->value() / 100.0;
    ui->koffUpr->setText(QString::number(mControllerManager->koffupr));
}

void MainWindow::on_koff1_clicked()
{
    ui->sliderUpr->setValue(100);
    ui->sliderUpr->update();
    mControllerManager->koffupr = (double)ui->sliderUpr->value() / 100.0;
    ui->koffUpr->setText(QString::number(mControllerManager->koffupr));
}

void MainWindow::on_koffRight_clicked()
{
    double x = (double)ui->sliderUpr->value();
    x++;
    ui->sliderUpr->setValue(x);
    ui->sliderUpr->update();
    mControllerManager->koffupr = (double)ui->sliderUpr->value() / 100.0;
    ui->koffUpr->setText(QString::number(mControllerManager->koffupr));
}


/*отображение режима*/
void MainWindow::setMode(Mode pMode)
{
    switch (pMode) {
    case MODE_AUTO:
        ui->mode->setText(tr("AUTO"));
        break;
    case MODE_SEMI_AUTO:
        ui->mode->setText(tr("SEMI AUTO"));
        break;
    case MODE_TRACKING:
        break;
    case MODE_MARCH:
         break;

    }
}

/*отображение данных последней сохраненной координаты*/
void MainWindow::setLastSaved(long pPositionX, long pPositionY, int pRange,  int pNumberAim)
{
    float x = static_cast<float>(pPositionX) / DEGREE_TO_CENTIDEGREE;
    float y = static_cast<float>(pPositionY) / DEGREE_TO_CENTIDEGREE;
    y=y;
    pRange=pRange;
    pNumberAim=pNumberAim;
    if (x < 0.0f)
        x += 360.0f;
}

/*обработчик меню установка длины очереди*/
void MainWindow::on_actionBurst_length_triggered()
{
    mControllerManager->setPause(true);
    setInterfaceInteractivity(false);
    mBurstLengthDialog.setControllerManager(mControllerManager);
    mBurstLengthDialog.exec();
    setInterfaceInteractivity(true);
    mControllerManager->setPause(false);
}

/*обработчик меню установка количества боезапаса*/
void MainWindow::on_actionAmmunition_triggered()
{
    mControllerManager->setPause(true);
    setInterfaceInteractivity(false);
    AmmunitionDialog AmmunitionDialogDialog(this, cur_weapon);

    if (flagInversia)
    {
        AmmunitionDialogDialog.setStyleSheet(COLOR_FON_NORMA);
        AmmunitionDialogDialog.colorInversia(true);
    }
    else
    {
        AmmunitionDialogDialog.setStyleSheet(COLOR_FON_INVERSIA);
        AmmunitionDialogDialog.colorInversia(false);
    }

    AmmunitionDialogDialog.setControllerManager(mControllerManager);
    AmmunitionDialogDialog.exec();
    setInterfaceInteractivity(true);
    mControllerManager->setPause(false);
}

/*обработчик ручного ввода дальности*/
void MainWindow::on_rangeInput_editingFinished()
{
    long lRange = ui->rangeInput->text().toLong();
    ui -> centralWidget ->setFocus();
    mControllerManager->setRange(lRange);
}

/*обработчик меню дежурный режим*/
void MainWindow::on_actionStand_by_mode_triggered()
{
    mVideoWidget->setOperationMode(false, false);
    mControllerManager->setStandbyMode();
    ui->actionStand_by_mode->setChecked(true);
    ui->actionOperation_mode->setChecked(false);
    mOptWidget->setOperationMode(false);
}

/*обработчик меню рабочий режим*/
void MainWindow::on_actionOperation_mode_triggered()
{
    mVideoWidget->setOperationMode(true, false);
    mControllerManager->setOperationMode();
    ui->actionOperation_mode->setChecked(true);
    ui->actionStand_by_mode->setChecked(false);
    mOptWidget->setOperationMode(true);
}

/*обработчик ручного ввода скорости ветра бокового*/
void MainWindow::on_windXInput_editingFinished()
{
    long lWind = ui->windXInput->text().toLong();
    ui -> centralWidget -> setFocus();
    mControllerManager->setWindX(lWind);
}

/*обработчик ручного ввода скорости ветра встречного*/
void MainWindow::on_windYInput_editingFinished()
{
    long lWind = ui->windYInput->text().toLong();
    ui -> centralWidget -> setFocus();
    mControllerManager->setWindY(lWind);
}

/*обработчик ручного ввода температуры*/
void MainWindow::on_temperatureInput_editingFinished()
{
    long lTemperature = ui->temperatureInput->text().toLong();
    ui -> centralWidget -> setFocus();
    mControllerManager->setThermo(lTemperature);
}

/*обработчик ручного ввода давления*/
void MainWindow::on_pressureInput_editingFinished()
{
    long lPressure = ui->pressureInput->text().toLong();
    ui -> centralWidget -> setFocus();
    mControllerManager->setPressure(lPressure, false);
}

/*обработчик меню выключение*/
void MainWindow::on_actionClose_triggered()
{
    mControllerManager->closeDevice();
    QApplication::exit(149);
}

/*действия при смене камеры и отображение типа текущей камеры*/
void MainWindow::setCameraMode(CameraMode pCameraMode)
{
    if (mOptWidget && mVideoWidget) {
        mOptWidget->setCameraMode(pCameraMode);
        mVideoWidget->setCameraMode(pCameraMode);
    }

    cur_camera = pCameraMode;
    switch (pCameraMode) {
    case NARROW_FORMAT:
        ui->camera->setText(tr("NFOV"));
        break;
    case WIDE_FORMAT:
        ui->camera->setText(tr("WFOV"));
        break;
    case THERMAL:
        ui->camera->setText(tr("IR"));
        break;
    case ZOOM_THERMAL:
        ui->camera->setText(tr("IR(X2)"));
        break;
    }
}

/*обработчик меню звуковой сигнал*/
void MainWindow::showSoundSignalDialog()
{
    setInterfaceInteractivity(false);
    mSoundSignalDialog->show();
}
/*обработчик меню юстировка*/
void MainWindow::showAdjustmentDialog()
{
    mControllerManager->setPause(true);
    setInterfaceInteractivity(false);
    ad.setControllerManager(mControllerManager);
    ad.exec();
    setInterfaceInteractivity(true);
    mControllerManager->setPause(false);
}

/*включение элементов интерфейса*/
void MainWindow::enableInterface()
{
    setInterfaceInteractivity(true);
}

/*установка параметров при первой установки соединения*/
void MainWindow::resetCamera()
{
    mOptWidget->setCameraMode(cur_camera);
}

void MainWindow::on_actionNormal_mode_triggered()
{
    mTrackingZala -> normalMode();
}

void MainWindow::on_actionPicture_in_picture_triggered()
{
    mTrackingZala -> pictureInPicture();
}

void MainWindow::on_actionSmall_picture_in_picture_triggered()
{
    mTrackingZala -> smallPictureInPicture();
}

void MainWindow::on_actionHorizontal_line_triggered()
{
    mTrackingZala -> horizontalLine();
}

void MainWindow::on_actionVertical_line_triggered()
{
    mTrackingZala -> verticalLine();
}

void MainWindow::on_actionDigital_stabilization_triggered()
{
    mTrackingZala -> digitalStabilization();
}

void MainWindow::on_actionPID_triggered()
{
    mPID->show();
}

void MainWindow::inversia(bool pFlag)
{
    if (pFlag)
    {
        paintNormal();

        mOptWidget -> invertCrossColor();
    }
    else
    {
        paintInversia();

        mOptWidget -> invertCrossColor();
    }
}


/*сброс периодического сигнала*/
void MainWindow::resetBeep()
{
    mBeeper->resetBeep();
}

/*проигрывание по кнопке тест из диалогового окна звукового сигнала*/
void MainWindow::testBeep(BeepState pBeepState)
{
    mBeeper->testPlay(pBeepState);
}

/*установка параметров звукового сигнала*/
void MainWindow::setBeepState(BeepState pBeepState)
{
    mBeeper->setBeepState(pBeepState);
    mBeeper->setBeepSignal((bool)pBeepState.is_beep_enabled);
}

/*инициализация при подключении ПП*/
void MainWindow::setActivatePlathorm(bool activate)
{
    if (activate) {
        mPlathormDir->setAngle(0);
        setPositionX(0);
        setPositionY(0);
        setSpeedX(0);
        setSpeedY(0);
        ActionList::getSingletonPtr()->clear();
        if (mControllerManager->isStabilization())
            setStabilization(false);
        mControllerManager->closeAutoMode();
        //Устанавливаем значения по умолчанию для поправок
        ui->pressureInput->setText("750");
        ui->rangeInput->setText("100");
        ui->temperatureInput->setText("15");
        ui->windXInput->setText("0");
        ui->windYInput->setText("0");
        mControllerManager->setPressure(750, false);
        mControllerManager->setRange(100);
        mControllerManager->setThermo(15);
        mControllerManager->setWindX(0);
        mControllerManager->setWindY(0);
        AimList::getSingletonPtr()->clear();
        ui->camera->setText(tr("WFOV"));
        mControllerManager->setCamera(WIDE_FORMAT);
        setBurstType(SHORT_BURST);
        setFireType(SINGLE_FIRE);
        setAutoMode(false);
        setMode(MODE_SEMI_AUTO);
        setMessage(ui->rotatingPlatformVoltage, "--.-");
    }
    mVideoWidget->setOperationMode(activate, !activate);
    ui->powerConsuptionMenu->setEnabled(activate);
    ui->armamentMenu->setEnabled(activate);
    ui->rangeInput->setEnabled(activate);
    ui->pressureInput->setEnabled(activate);
    ui->temperatureInput->setEnabled(activate);
    ui->windXInput->setEnabled(activate);
    ui->windYInput->setEnabled(activate);
    ui->actionAdjustment->setEnabled(activate);
    ui->alertMenu->setEnabled(activate);

    flagActivate = activate;

}

/*перерисовка элемента с соответствующим цветом*/
void MainWindow::setMessage(QLabel *label, QString msg, MessageType m_type)
{
    QPalette palette = label->palette();
    switch (m_type) {
    case neutral:
        palette.setColor(QPalette::WindowText, Qt::black);
        break;
    case safe:
        palette.setColor(QPalette::WindowText, Qt::green);
        break;
    case unsafe:
        palette.setColor(QPalette::WindowText, Qt::red);
        break;
    }
    label->setPalette(palette);
    label->setText(msg);
}

/*включение/выключение элементов интерфейса*/
void MainWindow::setInterfaceInteractivity(bool interactive)
{
    ui->stationMenu->setEnabled(interactive);
    ui->powerConsuptionMenu->setEnabled(interactive);
    ui->armamentMenu->setEnabled(interactive);
    ui->alertMenu->setEnabled(interactive);
    ui->rangeInput->setEnabled(interactive);
    ui->temperatureInput->setEnabled(interactive);
    ui->pressureInput->setEnabled(interactive);
    ui->windXInput->setEnabled(interactive);
    ui->windYInput->setEnabled(interactive);
}

void MainWindow::paintNormal()
{
    ui -> centralWidget -> setStyleSheet(COLOR_FON_NORMA);

    ui -> LogWidget -> setStyleSheet(COLOR_LIST_WIDGET_NORMA);

//Label name
    ui -> ammunitionGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> burstTypeLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> cameraLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> cockingLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> controlUnitVoltageLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> controlUnitVoltageMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> correctionsGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> fireLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> fireResolutionLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> fireTypeLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> horizontalAngleLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> horizontalSpeedLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> modeGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> modeLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> platformDirectionGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> powerSupplyGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> pressureLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> pressureMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> rangeLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> rangeMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> rotatingPlatformVoltageLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> rotatingPlatformVoltageMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> speedGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> stabilizationLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> temperatureLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> temperatureMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> trackingLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> verticalAngleLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> verticalSpeedLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> weaponStateGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> weaponTypeGroupLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> windLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> windXArrowLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> windXMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> windYArrowLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> windYMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> wiperLabel -> setStyleSheet(COLOR_LABEL_NORMA);
    ui -> labelState -> setStyleSheet(COLOR_LABEL_NORMA_ACTIVATE);
    ui -> verticalAngleSlider -> setStyleSheet(COLOR_SLIDER_NORMA);

//Label Data
    ui -> ammunition -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> burstType -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> camera -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> cocking -> setStyleSheet(COLOR_LABEL_NORMA_ACTIVATE);
    ui -> controlUnitVoltage -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> fire -> setStyleSheet(COLOR_LABEL_NORMA_ACTIVATE);
    ui -> fireResolution -> setStyleSheet(COLOR_LABEL_NORMA_ACTIVATE);
    ui -> fireType -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> horizontalAngle -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> horizontalSpeed -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> mode -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> powerSupplyInfo -> setStyleSheet(COLOR_LABEL_NORMA_ACTIVATE);
    ui -> rotatingPlatformVoltage -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> stabilization -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> tracking -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> verticalAngle -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> verticalSpeed -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> weaponType -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
    ui -> wiper -> setStyleSheet(COLOR_LABEL_NORMA_DATA);

//Button
    ui -> inversiaButton -> setStyleSheet(PICTURE_NIGHT_BUTTON);

//Line
    ui -> line_1 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_2 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_3 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_4 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_5 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_6 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_7 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_8 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_10 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_11 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_12 -> setStyleSheet(COLOR_LINE_NORMA);
    ui -> line_13 -> setStyleSheet(COLOR_LINE_NORMA);

//LINE_EDIT
    ui -> pressureInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
    ui -> rangeInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
    ui -> temperatureInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
    ui -> windXInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
    ui -> windYInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

//MENU
    ui -> menuBar -> setStyleSheet(COLOR_MENU_BAR_NORMA);
    ui -> stationMenu -> setStyleSheet(COLOR_MENU_NORMA);
    ui -> powerConsuptionMenu -> setStyleSheet(COLOR_MENU_NORMA);
    ui -> armamentMenu -> setStyleSheet(COLOR_MENU_NORMA);
    ui -> alertMenu -> setStyleSheet(COLOR_MENU_NORMA);

    ad.setStyleSheet(COLOR_FON_NORMA);
    ad.colorInversia(true);

    mAutoModeTypeDialog.setStyleSheet(COLOR_FON_NORMA);
    mAutoModeTypeDialog.colorInversia(true);

    mBurstLengthDialog.setStyleSheet(COLOR_FON_NORMA);
    mBurstLengthDialog.colorInversia(true);

    mPID->setStyleSheet(COLOR_FON_NORMA);
    mPID->colorInversia(true);

    mSoundSignalDialog -> setStyleSheet(COLOR_FON_NORMA);
    mSoundSignalDialog -> colorInversia(true);

    mPlathormDir -> setStyleSheet(COLOR_FON_NORMA);
    mPlathormDir -> inversia(true);

    mVideoWidget -> getFlagInversia(true);
}


void MainWindow::paintInversia()
{
    ui -> centralWidget -> setStyleSheet(COLOR_FON_INVERSIA);

    ui -> LogWidget -> setStyleSheet(COLOR_LIST_WIDGET_INVERSIA);

//Label name
    ui -> ammunitionGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> burstTypeLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> cameraLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> cockingLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> controlUnitVoltageLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> controlUnitVoltageMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> correctionsGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> fireLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> fireResolutionLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> fireTypeLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> horizontalAngleLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> horizontalSpeedLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> modeGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> modeLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> platformDirectionGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> powerSupplyGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> pressureLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> pressureMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> rangeLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> rangeMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> rotatingPlatformVoltageLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> rotatingPlatformVoltageMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> speedGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> stabilizationLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> temperatureLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> temperatureMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> trackingLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> verticalAngleLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> verticalSpeedLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> weaponStateGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> weaponTypeGroupLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> windLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> windXArrowLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> windXMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> windYArrowLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> windYMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);
    ui -> wiperLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);



//Label Data
    ui -> ammunition -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> burstType -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> camera -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> cocking -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> controlUnitVoltage -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> fire -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> fireResolution -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> fireType -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> horizontalAngle -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> horizontalSpeed -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> mode -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> powerSupplyInfo -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> rotatingPlatformVoltage -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> stabilization -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> tracking -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> verticalAngle -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> verticalSpeed -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> weaponType -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
    ui -> wiper -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);

//Button
    ui -> inversiaButton -> setStyleSheet(PICTURE_DAY_BUTTON);

//Line
    ui -> line_1 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_2 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_3 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_4 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_5 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_6 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_7 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_8 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_10 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_11 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_12 -> setStyleSheet(COLOR_LINE_INVERSIA);
    ui -> line_13 -> setStyleSheet(COLOR_LINE_INVERSIA);

//LINE_EDIT
    ui -> pressureInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
    ui -> rangeInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
    ui -> temperatureInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
    ui -> windXInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
    ui -> windYInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

//MENU
    ui -> menuBar -> setStyleSheet(COLOR_MENU_BAR_INVERSIA);
    ui -> stationMenu -> setStyleSheet(COLOR_MENU_INVERSIA);
    ui -> powerConsuptionMenu -> setStyleSheet(COLOR_MENU_INVERSIA);
    ui -> armamentMenu -> setStyleSheet(COLOR_MENU_INVERSIA);
    ui -> alertMenu -> setStyleSheet(COLOR_MENU_INVERSIA);

    ad.setStyleSheet(COLOR_FON_INVERSIA);
    ad.colorInversia(false);

    mAutoModeTypeDialog.setStyleSheet(COLOR_FON_INVERSIA);
    mAutoModeTypeDialog.colorInversia(false);

    mBurstLengthDialog.setStyleSheet(COLOR_FON_INVERSIA);
    mBurstLengthDialog.colorInversia(false);

    mPID -> setStyleSheet(COLOR_FON_INVERSIA);
    mPID -> colorInversia(false);

    mSoundSignalDialog -> setStyleSheet(COLOR_FON_INVERSIA);
    mSoundSignalDialog -> colorInversia(false);

    mPlathormDir -> setStyleSheet(COLOR_FON_INVERSIA);
    mPlathormDir -> inversia(false);

    mVideoWidget -> getFlagInversia(false);
}


void MainWindow::on_inversiaButton_clicked()
{
    if (flagInversia)
    {
        flagInversia = false;
        inversia(false);
        mTrackingZala->acTrack(true);
    }
    else
    {
        flagInversia = true;
        inversia(true);
        mTrackingZala->acTrack(false);
    }
}

void MainWindow::on_actionKORD_triggered()
{
    if (cur_weapon == WEAPON_KORD)
    {
        ui -> actionKORD -> setChecked(true);
    }
    mControllerManager->setWeapon(cur_weapon = WEAPON_KORD);
    mControllerManager->correctionToFire();
    mControllerManager->setAmmunitionCounter(0, true);
    ui->weaponType->setText(tr("SM-5"));
    ui->actionKORD->setChecked(true);
    ui->actionPKT->setChecked(false);
    ui->actionAGS_17->setChecked(false);
    ui->actionAGS_40->setChecked(false);
}

void MainWindow::on_actionPKT_triggered()
{
    if (cur_weapon == WEAPON_PKT)
    {
        ui->actionPKT->setChecked(true);
        return;
    }
    mControllerManager->setWeapon(cur_weapon = WEAPON_PKT);
    mControllerManager->correctionToFire();
    mControllerManager->setAmmunitionCounter(0, true);
    ui->weaponType->setText(tr("ПКТ"));
    ui->actionKORD->setChecked(false);
    ui->actionPKT->setChecked(true);
    ui->actionAGS_17->setChecked(false);
    ui->actionAGS_40->setChecked(false);
}


void MainWindow::on_actionAGS_17_triggered()
{
    if (cur_weapon == WEAPON_AGS) {
        ui->actionAGS_17->setChecked(true);
        return;
    }
    mControllerManager->setWeapon(cur_weapon = WEAPON_AGS);
    mControllerManager->correctionToFire();
    mControllerManager->setAmmunitionCounter(0, true);
    ui->weaponType->setText("АГС-17");
    mControllerManager->setAmmunitionCounter(0, true);
    ui->actionKORD->setChecked(false);
    ui->actionPKT->setChecked(false);
    ui->actionAGS_17->setChecked(true);
    ui->actionAGS_40->setChecked(false);
}

void MainWindow::on_actionAGS_40_triggered()
{
    if (cur_weapon == WEAPON_AG40) {
           ui->actionAGS_40->setChecked(true);
           return;
       }
    mControllerManager->setWeapon(cur_weapon = WEAPON_AG40);
    mControllerManager->correctionToFire();
    mControllerManager->setAmmunitionCounter(0, true);
    ui->weaponType->setText("АГС-40");
    ui->actionKORD->setChecked(false);
    ui->actionPKT->setChecked(false);
    ui->actionAGS_17->setChecked(false);
    ui->actionAGS_40->setChecked(true);
}
