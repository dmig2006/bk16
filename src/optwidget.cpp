#include "optwidget.h"
#include "ui_optwidget.h"
#include "mainwindow.h"

/*Конструктор класса*/
OptWidget::OptWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::OptWidget)
{
    ui->setupUi(this);
    QPixmap curPix;
    curPix.load(":/cross.png");
    QCursor cur(curPix);
    setCursor(cur);

    isPressRect = false;
    is_cocking = false;

    flagLabel = false;
    isInvertColor = false;

    mRectTracking = QRect(0, 0, 100, 100);

    mGraphicsWhite.pixmapCrossWide.load(":/white/cross_shz.png");
    mGraphicsWhite.pixmapCrossNarrow.load(":/white/cross_uz.png");
    mGraphicsWhite.pixmapCrossThermo.load(":/white/cross_teplo.png");
    mGraphicsWhite.pixmapCrossZoomThermo.load(":/white/cross_teplo_zoom.png");
    mGraphicsWhite.pixmapOptNarrow.load(":/white/opt_uz.png");
    mGraphicsWhite.pixmapOptThermo.load(":/white/opt_teplo.png");
    mGraphicsWhite.pixmapOptZoomThermo.load(":/white/opt_teplo_zoom.png");
    mGraphicsWhite.pixmapOptWide.load(":/white/opt_shz.png");
    mGraphicsWhite.pixmapRectNarrow.load(":/white/ramka_uz.png");
    mGraphicsWhite.pixmapAim.load(":/white/target.png");
    mGraphicsWhite.pixmapCrossTg.load(":/white/cross.png");
    mGraphicsWhite.pixmapTriangleHor.load(":/white/triangle_hor.png");
    mGraphicsWhite.pixmapTriangleVert.load(":/white/triangle_vert.png");
    mGraphicsWhite.pixmapCenter.load(":/white/center.png");
    mGraphicsWhite.pixmapOptRange.load(":/white/opt_range.png");
    mGraphicsWhite.pixmapLogo.load(":/logo.png");

    mGraphicsBlack.pixmapCrossWide.load(":/black/cross_shz.png");
    mGraphicsBlack.pixmapCrossNarrow.load(":/black/cross_uz.png");
    mGraphicsBlack.pixmapCrossThermo.load(":/black/cross_teplo.png");
    mGraphicsBlack.pixmapCrossZoomThermo.load(":/black/cross_teplo_zoom.png");
    mGraphicsBlack.pixmapOptNarrow.load(":/black/opt_uz.png");
    mGraphicsBlack.pixmapOptThermo.load(":/black/opt_teplo.png");
    mGraphicsBlack.pixmapOptZoomThermo.load(":/black/opt_teplo_zoom.png");
    mGraphicsBlack.pixmapOptWide.load(":/black/opt_shz.png");
    mGraphicsBlack.pixmapRectNarrow.load(":/black/ramka_uz.png");
    mGraphicsBlack.pixmapAim.load(":/black/target.png");
    mGraphicsBlack.pixmapCrossTg.load(":/black/cross.png");
    mGraphicsBlack.pixmapTriangleHor.load(":/black/triangle_hor.png");
    mGraphicsBlack.pixmapTriangleVert.load(":/black/triangle_vert.png");
    mGraphicsBlack.pixmapCenter.load(":/black/center.png");
    mGraphicsBlack.pixmapOptRange.load(":/black/opt_range.png");
    mGraphicsBlack.pixmapLogo.load(":/logo.png");

    mGraphics = mGraphicsBlack;
    color = Qt::black;
    idColor = 0;
    pixmapL = mGraphics.pixmapLogo;

    mCorrectionX = 0;
    mCorrectionY = 0;

    mPositionEncoderX = 0;
    mPositionEncoderY = 0;

    mJustX = 0;
    mJustY = 0;

    mRectWidth = 100;
    mRectHeight = 100;

    mRange = 100;

    mHeightOptRange = 100;
    mVisibleOptRange = false;

    mTracking = false;
    mStabilization = false;
    mConnect = false;
    isVisibleTracking = false;

    mCameraMode = WIDE_FORMAT;
    setCameraMode(mCameraMode);

    isFilter = false;
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(update()));
    timer->start(50);
    mAimType = HOME;
    updateRect();
    need_to_show_pixmap_opt = true;
    is_operation_mode = true;
}

/*установка флага рабочий режим*/
void OptWidget::setOperationMode(bool enabled)
{
    is_operation_mode = enabled;
}

/*установка флага взвод*/
void OptWidget::setCocking(bool enabled)
{
    is_cocking = enabled;
}

/*Деструктор класса*/
OptWidget::~OptWidget()
{
    delete ui;
}

/*передача координат центра рамки сопровождения*/
void OptWidget::setCenterMassTracking(int x, int y)
{
    mControllerManager->setCenterMassTracking(x, y);
}

/*получение указателя на ControllerManager*/
void OptWidget::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
}

/*получение координат рамки сопровождения*/
void OptWidget::setRects(QRect pRect_t1, QRect pRect_t2)
{
    mMutex.lock();
    mRect_t1 = pRect_t1;
    mRect_t2 = pRect_t2;
    mMutex.unlock();
}

/*включение/выключение фильтра на автомате сопровождения*/
bool OptWidget::setFilter(bool state)
{
    isFilter = state;
    return true;
}

/*инвертирование цвета перекрестий*/
void OptWidget::invertCrossColor()
{
    mMutex.lock();
    if (idColor == 0) {
        idColor = 1;
        color = Qt::white;
        mGraphics = mGraphicsWhite;
    } else {
        idColor = 0;
        color = Qt::black;
        mGraphics = mGraphicsBlack;
    }
    switch (mCameraMode) {
    case NARROW_FORMAT:
        pixmapCross = mGraphics.pixmapCrossNarrow;
        pixmapOpt = mGraphics.pixmapOptNarrow;
        break;
    case WIDE_FORMAT:
        pixmapCross = mGraphics.pixmapCrossWide;
        pixmapOpt = mGraphics.pixmapOptWide;
        pixmapNarrow = mGraphics.pixmapRectNarrow;
        break;
    case THERMAL:
        pixmapCross = mGraphics.pixmapCrossThermo;
        pixmapOpt = mGraphics.pixmapOptThermo;
        break;
    case ZOOM_THERMAL:
        pixmapCross = mGraphics.pixmapCrossZoomThermo;
        pixmapOpt = mGraphics.pixmapOptZoomThermo;
        break;
    }
    mMutex.unlock();
    updateRect();
}

void OptWidget::setBlockPP(bool enabled)
{
        is_cocking = enabled;
}

/*обновление рамки сопровождения*/
void OptWidget::updateRect()
{
    float w = 0.0;
    float h = 0.0;
    QString strColor;
    if (idColor == 1)
        strColor = "WHITE";
    else
        strColor = "BLACK";
    switch (mAimType) {
    case HOME:
        ui->label->setText(TEXT_COLOR(strColor, "ЦЕЛЬ БОЛЬШАЯ"));
        w = HOME_SIZE_W;
        h = HOME_SIZE_H;
        break;
    case CAR:
        ui->label->setText(TEXT_COLOR(strColor, "ЦЕЛЬ МАЛЕНЬКАЯ"));
        w = CAR_SIZE_W;
        h = CAR_SIZE_H;
        break;
    }
    mMutex.lock();
    if (mCameraMode == WIDE_FORMAT) {
        mRectWidth = (atan((double)(w / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_WIDTH / ANGLE_WIDE_X) * 2.0;
        mRectHeight = (atan((double)(h / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_HEIGHT / ANGLE_WIDE_Y) * 2.0;
    }
    if (mCameraMode == NARROW_FORMAT) {
        mRectWidth = (atan((double)(w / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_WIDTH / ANGLE_NARROW_X) * 2.0;
        mRectHeight = (atan((double)(h / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_HEIGHT / ANGLE_NARROW_Y) * 2.0;
    }
    if (mCameraMode == THERMAL) {
        mRectWidth = (atan((double)(w / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_WIDTH / ANGLE_THERMAL_X) * 2.0;
        mRectHeight = (atan((double)(h / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_HEIGHT / ANGLE_THERMAL_Y) * 2.0;
    }
    if (mCameraMode == ZOOM_THERMAL) {
        mRectWidth = (atan((double)(w / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_WIDTH / ANGLE_ZOOM_THERMAL_X) * 2.0;
        mRectHeight = (atan((double)(h / 2.0) / (double)mRange) * 180 / M_PI)
                * ((double)DEFAULT_HEIGHT / ANGLE_ZOOM_THERMAL_Y) * 2.0;
    }
    if (mRectWidth > DEFAULT_WIDTH / 3)
        mRectWidth = DEFAULT_WIDTH / 3;
    if (mRectHeight > DEFAULT_HEIGHT / 3)
        mRectHeight = DEFAULT_HEIGHT / 3;
    if (mRectWidth < MIN_RECT_WIDTH)
        mRectWidth = MIN_RECT_WIDTH;
    if (mRectHeight < MIN_RECT_HEIGHT)
        mRectHeight = MIN_RECT_HEIGHT;
    mMutex.unlock();
}

/*установка типа цели для сопровождения*/
void OptWidget::setAimType(AimType pAimType)
{
    mAimType = pAimType;
    updateRect();
}

/*увеличение шкалы визуального дальномера*/
void OptWidget::optRangeUp()
{
    ++mHeightOptRange;
    updateOptRange(mHeightOptRange, mCameraMode);
}

/*уменьшение шкалы визуального дальномера*/
void OptWidget::optRangeDown()
{
    if (mHeightOptRange > 0) {
        --mHeightOptRange;
        updateOptRange(mHeightOptRange, mCameraMode);
    }
}

/*включение/выключение визуального дальномера*/
void OptWidget::setVisibleOptRange(bool state)
{
    mMutex.lock();
    mVisibleOptRange = state;
    mMutex.unlock();
}

/*установка дальности*/
void OptWidget::setRange(int pRange)
{
    mRange = pRange;
    updateRect();
}

/*установка типа камеры*/
void OptWidget::setCameraMode(CameraMode pCameraMode)
{
    mMutex.lock();
    isVisibleTracking = false;
    mCameraMode = pCameraMode;
    switch (mCameraMode) {
    case NARROW_FORMAT:
        pixmapCross = mGraphics.pixmapCrossNarrow;
        pixmapOpt = mGraphics.pixmapOptNarrow;
        break;
    case WIDE_FORMAT:
        pixmapCross = mGraphics.pixmapCrossWide;
        pixmapOpt = mGraphics.pixmapOptWide;
        pixmapNarrow = mGraphics.pixmapRectNarrow;
        break;
    case THERMAL:
        pixmapCross = mGraphics.pixmapCrossThermo;
        pixmapOpt = mGraphics.pixmapOptThermo;
        break;
    case ZOOM_THERMAL:
        pixmapCross = mGraphics.pixmapCrossZoomThermo;
        pixmapOpt = mGraphics.pixmapOptZoomThermo;
        break;
    }
    mMutex.unlock();
    updateRect();
}

/*удаление рамки сопровождения*/
void OptWidget::setNullRect()
{
    mRect_t1.setWidth(0);
    mRect_t2.setHeight(0);
    mRect_t1.setHeight(0);
    mRect_t2.setWidth(0);
}

/*отрисовка элементов на видеоизображении*/
void OptWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    mMutex.lock();
    /*draw tracking rects*/
    if (mTracking && isVisibleTracking) {
        painter.setPen(QColor(0, 255, 0));
        painter.drawRect(mRect_t1);
        painter.setPen(QColor(0, 0, 255));
        painter.drawRect(mRect_t2);
    } else {
        setNullRect();
    }

    if (flagLabel)
        ui -> label -> show();
    else
        ui -> label -> hide();

    /*draw main cross*/
    painter.drawImage(600, 14, pixmapL); //вывод логотипа КК (хх10 , yy14)
    painter.drawImage(mJustX, mJustY, pixmapCross);
    painter.drawImage(width() / 2 - mGraphics.pixmapCenter.width() / 2,
                      height() / 2 - mGraphics.pixmapCenter.height() / 2,
                      mGraphics.pixmapCenter);
    if (need_to_show_pixmap_opt)
        painter.drawImage(15, 300, pixmapOpt);
    if (mVisibleOptRange)
        painter.drawImage(width() / 2 - mGraphics.pixmapOptRange.width() / 2 + mJustX,
                          height() / 2 - mGraphics.pixmapOptRange.height() / 2 + mJustY - mHeightOptRange,
                          mGraphics.pixmapOptRange);
    if (mCameraMode == WIDE_FORMAT)
           painter.drawImage(width() / 2 - pixmapNarrow.width() / 2,
                             height() / 2 - pixmapNarrow.height() / 2,
                             pixmapNarrow);
    painter.drawImage(int(mCorrectionX) - mGraphics.pixmapCrossTg.width() / 2 + mJustX,
                      int(mCorrectionY) - mGraphics.pixmapCrossTg.height() / 2 + mJustY,
                      mGraphics.pixmapCrossTg);
    /*draw aim*/
    if (!mTracking && is_operation_mode) {
        QImage lPixmapAim;
        lPixmapAim = mGraphics.pixmapAim;
        for (int i = 0; i < AimList::getSingletonPtr()->size(); ++i) {
            Aim *lAim = AimList::getSingletonPtr()->get(i);
            if (lAim) {
                long PosX = 0;
                long PosY = 0;
                long DeltaPositionX = 0;
                long UnitPositionX = 0;
                UnitPositionX = mControllerManager->getUnitPositionX(mPositionEncoderX);
                DeltaPositionX = mControllerManager->getDeltaX(lAim->mPositionX, UnitPositionX);
                switch (mCameraMode) {
                case WIDE_FORMAT:
                    PosX = static_cast<long>(static_cast<double>(-INVERT_SPEED_X)
                                             * static_cast<double>(-DeltaPositionX)
                                             * static_cast<double>(-DEFAULT_WIDTH)
                                             / ANGLE_WIDE_X / 100.0
                                             + static_cast<double>(DEFAULT_WIDTH) / 2.0);
                    PosY = static_cast<long>(static_cast<double>(-INVERT_SPEED_Y)
                                             * static_cast<double>(-mPositionEncoderY
                                                                   - lAim->mPositionY)
                                             * static_cast<double>(DEFAULT_HEIGHT)
                                             / ANGLE_WIDE_Y / 100.0
                                             + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
                    break;
                case NARROW_FORMAT:
                    PosX = static_cast<long>(static_cast<double>(-INVERT_SPEED_X)
                                             * static_cast<double>(-DeltaPositionX)
                                             * static_cast<double>(-DEFAULT_WIDTH)
                                             / ANGLE_NARROW_X / 100.0
                                             + static_cast<double>(DEFAULT_WIDTH) / 2.0);
                    PosY = static_cast<long>(static_cast<double>(-INVERT_SPEED_Y)
                                             * static_cast<double>(-mPositionEncoderY
                                                                   - lAim->mPositionY)
                                             * static_cast<double>(DEFAULT_HEIGHT)
                                             / ANGLE_NARROW_Y / 100.0
                                             + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
                    break;
                case THERMAL:
                    PosX = static_cast<long>(static_cast<double>(-INVERT_SPEED_X)
                                             * static_cast<double>(-DeltaPositionX)
                                             * static_cast<double>(-DEFAULT_WIDTH)
                                             / ANGLE_THERMAL_X / 100.0
                                             + static_cast<double>(DEFAULT_WIDTH) / 2.0);
                    PosY = static_cast<long>(static_cast<double>(-INVERT_SPEED_Y)
                                             * static_cast<double>(-mPositionEncoderY
                                                                   - lAim->mPositionY)
                                             * static_cast<double>(DEFAULT_HEIGHT)
                                             / ANGLE_THERMAL_Y / 100.0
                                             + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
                    break;
                case ZOOM_THERMAL:
                    PosX = static_cast<long>(static_cast<double>(-INVERT_SPEED_X)
                                             * static_cast<double>(-DeltaPositionX)
                                             * static_cast<double>(-DEFAULT_WIDTH)
                                             / ANGLE_ZOOM_THERMAL_X / 100.0
                                             + static_cast<double>(DEFAULT_WIDTH) / 2.0);
                    PosY = static_cast<long>(static_cast<double>(-INVERT_SPEED_Y)
                                             * static_cast<double>(-mPositionEncoderY
                                                                   - lAim->mPositionY)
                                             * static_cast<double>(DEFAULT_HEIGHT)
                                             / ANGLE_ZOOM_THERMAL_Y / 100.0
                                             + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
                    break;
                }
                painter.drawImage(int(PosX) - lPixmapAim.width() / 2 + mJustX,
                                  int(PosY) - lPixmapAim.height() / 2 + mJustY,
                                  lPixmapAim);
                painter.setPen(color);
                painter.drawText(QRect(int(PosX) - lPixmapAim.width() / 2 + 2 + mJustX,
                                       int(PosY) - lPixmapAim.height() / 2 + mJustY,
                                       15, 15),
                                 QString::number(lAim->mTarget));
            }
        }
    }
    if (mTracking) {
        painter.setPen(QColor(0, 0, 255));
        int m_x = cursor().pos().x() - geometry().x();
        int m_y = cursor().pos().y() - geometry().y();
        int x = m_x - mRectWidth / 2;
        int y = m_y - mRectHeight / 2;
        if (m_x + mRectWidth / 2 >= DEFAULT_WIDTH)
            x = DEFAULT_WIDTH - mRectWidth - 1;
        if (m_x <= mRectWidth / 2)
            x = 0;
        if (m_y + mRectHeight / 2 >= DEFAULT_HEIGHT)
            y = DEFAULT_HEIGHT - mRectHeight - 1;
        if (m_y <= mRectHeight / 2)
            y = 0;
        if (is_cursor_in_area) {
            mRectTracking = QRect(x, y, mRectWidth, mRectHeight);
            painter.setPen(color);
            painter.drawRect(mRectTracking);
        }
    }
    painter.end();
    mMutex.unlock();
}

/*действия при нажатии кнопки трекбола/сенсорного экрана*/
void OptWidget::mousePressEvent(QMouseEvent *event)
{
    int lJustX = 0;
    int lJustY = 0;
    float kofX = 0.0f;
    float kofx = 0.0f;
    float kofy = 0.0f;
    mMutex.lock();
    lJustX = mJustX;
    lJustY = mJustY;
    bool isTracking = mTracking;
    CameraMode lCameraMode = mCameraMode;
    mMutex.unlock();
    if (!isTracking)
    {
        if (event->button() == Qt::LeftButton
                || event->button() == Qt::RightButton)
        {
            int lX = event->x();
            int lY = event->y();
            long lConvertX = 0;
            long lConvertY = 0;
            long aC_ConvertX = 0;
            long aC_ConvertY = 0;

            lX = DEFAULT_WIDTH / 2 - lX + lJustX;
            lY = DEFAULT_HEIGHT / 2 - lY + lJustY;
            switch (lCameraMode)
            {
                case NARROW_FORMAT:
                    lConvertX = static_cast<long>(static_cast<double>(lX) * ANGLE_NARROW_X * 100.0  / static_cast<double>(DEFAULT_WIDTH));
                    lConvertY = static_cast<long>(static_cast<double>(lY) * ANGLE_NARROW_Y * 100.0  / static_cast<double>(DEFAULT_HEIGHT));

                    kofX = 8.449f;
                    kofx = 7.928f;
                    kofy = 6.863f;
                    if (lConvertX >= 0)
                    {
                        aC_ConvertX = static_cast<long>((((-127.0f/710.0f)*static_cast<double>(lConvertX)*kofX)));
                        if (aC_ConvertX <= -127)
                            aC_ConvertX = -127;
                    }
                    else
                    {
                        aC_ConvertX = static_cast<long>(((((127.0f/710.0f)*static_cast<double>(-1*lConvertX)*kofx))));
                        if (aC_ConvertX >= 127)
                            aC_ConvertX = 127;
                    }


                    if (lConvertY >= 0)
                    {
                         aC_ConvertY = static_cast<long>((((127.0f/530.0f)*static_cast<double>(lConvertY)*kofy)));
                        if (aC_ConvertY >= 127)
                            aC_ConvertY = 127;
                    }
                    else
                    {
                        aC_ConvertY = static_cast<long>(((((127.0f/530.0f)*static_cast<double>(lConvertY)*kofy))));
                        if (aC_ConvertY <= -127)
                            aC_ConvertY = -110;
                    }

                    emit move_Ac_Target(aC_ConvertX,aC_ConvertY); // подача сигнала на абсолютное перемещение рамки
                    break;

                case WIDE_FORMAT:
                    lConvertX = static_cast<long>(static_cast<double>(lX) * ANGLE_WIDE_X * 100.0    / static_cast<double>(DEFAULT_WIDTH));
                    lConvertY = static_cast<long>(static_cast<double>(lY) * ANGLE_WIDE_Y * 100.0    / static_cast<double>(DEFAULT_HEIGHT));

                    /*Блок рассчета изменения координат рамки для широкого поля зрения камеры*/
                    kofX = 1.428f;
                    kofx = 1.34f;
                    kofy = 1.16f;
                    if (lConvertX >= 0)
                    {
                        aC_ConvertX = static_cast<long>((((-127.0f/710.0f)*static_cast<double>(lConvertX)*kofX)));
                        if (aC_ConvertX <= -127)
                            aC_ConvertX = -127;
                    }
                    else
                    {
                        aC_ConvertX = static_cast<long>(((((127.0f/710.0f)*static_cast<double>(-1*lConvertX)*kofx))));
                        if (aC_ConvertX >= 127)
                            aC_ConvertX = 127;
                    }


                    if (lConvertY >= 0)
                    {
                        aC_ConvertY = static_cast<long>((((127.0f/530.0f)*static_cast<double>(lConvertY)*kofy)));
                        if (aC_ConvertY >= 127)
                            aC_ConvertY = 127;
                    }
                    else
                    {
                        aC_ConvertY = static_cast<long>(((((127.0f/530.0f)*static_cast<double>(lConvertY)*kofy))));
                        if (aC_ConvertY <= -127)
                            aC_ConvertY = -110;
                    }

                    emit move_Ac_Target(aC_ConvertX,aC_ConvertY); // подача сигнала на абсолютное перемещение рамки

                    break;

                case THERMAL:
                    lConvertX = static_cast<long>(static_cast<double>(lX) * ANGLE_THERMAL_X * 100.0/ static_cast<double>(DEFAULT_WIDTH));
                    lConvertY = static_cast<long>(static_cast<double>(lY) * ANGLE_THERMAL_Y * 100.0/ static_cast<double>(DEFAULT_HEIGHT));

                    kofX = 3.000f;
                    kofx = 2.825f;
                    kofy = 2.385f;
                    if (lConvertX >= 0)
                    {
                        aC_ConvertX = static_cast<long>((((-127.0f/640.0f)*static_cast<double>(lConvertX)*kofX)));
                        if (aC_ConvertX <= -127)
                            aC_ConvertX = -127;
                    }
                    else
                    {
                        aC_ConvertX = static_cast<long>(((((127.0f/640.0f)*static_cast<double>(-1*lConvertX)*kofx))));
                        if (aC_ConvertX >= 127)
                            aC_ConvertX = 127;
                    }


                    if (lConvertY >= 0)
                    {
                        aC_ConvertY = static_cast<long>((((127.0f/480.0f)*static_cast<double>(lConvertY)*kofy)));
                        if (aC_ConvertY >= 127)
                            aC_ConvertY = 127;
                    }
                    else
                    {
                    aC_ConvertY = static_cast<long>(((((127.0f/480.0f)*static_cast<double>(lConvertY)*kofy))));
                    if (aC_ConvertY <= -127)
                        aC_ConvertY = -110;
                    }

                    emit move_Ac_Target(aC_ConvertX,aC_ConvertY); // подача сигнала на абсолютное перемещение рамки
                    break;

                case ZOOM_THERMAL:
                    lConvertX = static_cast<long>(static_cast<double>(lX) * ANGLE_ZOOM_THERMAL_X * 100.0 / static_cast<double>(DEFAULT_WIDTH));
                    lConvertY = static_cast<long>(static_cast<double>(lY) * ANGLE_ZOOM_THERMAL_Y * 100.0 / static_cast<double>(DEFAULT_HEIGHT));

                    kofX = 6.000f;
                    kofx = 5.750f;
                    kofy = 4.770f;
                    if (lConvertX >= 0)
                    {
                        aC_ConvertX = static_cast<long>((((-127.0f/640.0f)*static_cast<double>(lConvertX)*kofX)));
                        if (aC_ConvertX <= -127)
                            aC_ConvertX = -127;
                    }
                    else
                    {
                        aC_ConvertX = static_cast<long>(((((127.0f/640.0f)*static_cast<double>(-1*lConvertX)*kofx))));
                        if (aC_ConvertX >= 127)
                            aC_ConvertX = 127;
                    }


                    if (lConvertY >= 0)
                    {
                        aC_ConvertY = static_cast<long>((((127.0f/480.0f)*static_cast<double>(lConvertY)*kofy)));
                        if (aC_ConvertY >= 127)
                            aC_ConvertY = 127;
                    }
                        else
                    {
                        aC_ConvertY = static_cast<long>(((((127.0f/480.0f)*static_cast<double>(lConvertY)*kofy))));
                        if (aC_ConvertY <= -127)
                            aC_ConvertY = -110;
                    }

                    emit move_Ac_Target(aC_ConvertX,aC_ConvertY); // подача сигнала на абсолютное перемещение рамки
                    break;
                }


            if ((!is_cocking &&
                 mControllerManager->getMode()!=MODE_AUTO) ||
                 !mControllerManager->enabled_as)

                mControllerManager->setPositionDeviceDrive(-lConvertX,lConvertY,false);
        }
    }
}

/*действия при отпускании кнопки трекбола/сенсорного экрана*/
void OptWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mMutex.lock();
    bool isTracking = mTracking;
    mMutex.unlock();
    if (isTracking) {
        if (event->button() == Qt::RightButton) {
            int lX = event->x();
            int lY = event->y();
            lX = lX - DEFAULT_WIDTH / 2;
            lY = lY - DEFAULT_HEIGHT / 2;
            mControllerManager->setCorrectionTracking(lX, lY);
        } else {
            mMutex.lock();
            isPressRect = false;
            isVisibleTracking = true;
            mRectTracking = norm(mRectTracking);
            mMutex.unlock();
            mControllerManager->resetCorrectionTracking();
        }
    }
}

/*установка флага входа курсора в область видеоизображения*/
void OptWidget::enterEvent(QEvent *)
{
    is_cursor_in_area = true;
}

/*установка флага выхода курсора из области видеоизображения*/
void OptWidget::leaveEvent(QEvent *)
{
    is_cursor_in_area = false;
}

/*расчет дальности визуального дальномера*/
void OptWidget::updateOptRange(int pHeinghtOptRange, CameraMode pCameraMode)
{
    long distance = 0;
    if (pCameraMode == WIDE_FORMAT)
        distance = long(5121.0 / double(pHeinghtOptRange) + 0.5);
    if (pCameraMode == NARROW_FORMAT)
        distance = long(32775.0 / double(pHeinghtOptRange) + 0.5);
    if (pCameraMode == THERMAL)
        distance = long((12783.3) / double(pHeinghtOptRange) + 0.5);
    if (pCameraMode == ZOOM_THERMAL)
        distance = long((25544.0) / double(pHeinghtOptRange) + 0.5);
    if (distance > 0 && distance < 2600)
        mControllerManager->setRange(distance);
}

/*установка типа сопровождения*/
void OptWidget::setTypeTracking(int pType)
{
    mMutex.lock();
    mTypeTracking = pType;
    mMutex.unlock();
}

/*установкка флага состояния сопровождения*/
void OptWidget::setTracking(bool state)
{
    mMutex.lock();
    mTracking = state;
    mMutex.unlock();
}

/*установка флага состояния стабилизации*/
void OptWidget::setStabilization(bool state)
{
    mMutex.lock();
    mStabilization = state;
    mMutex.unlock();
}

/*установка координат основного перекрестия*/
void OptWidget::setJustCross(long pJustX, long pJustY)
{
    mMutex.lock();
    mJustX = pJustX;
    mJustY = pJustY;
    mMutex.unlock();
}

/*пересчёт координат перектрестия баллистики из углов в пиксели*/
void OptWidget::setCorrectionCross(long pCorrectionX, long pCorrectionY)
{
    mMutex.lock();
    mCorrectionX = pCorrectionX;
    mCorrectionY = pCorrectionY;
    switch (mCameraMode) {
    case WIDE_FORMAT:
        mCorrectionX = static_cast<long>(static_cast<double>(mCorrectionX)
                                         * static_cast<double>(-DEFAULT_WIDTH)
                                         / ANGLE_WIDE_X / 100.0
                                         + static_cast<double>(DEFAULT_WIDTH) / 2.0);
        mCorrectionY = static_cast<long>(static_cast<double>(mCorrectionY)
                                         * static_cast<double>(DEFAULT_HEIGHT)
                                         / ANGLE_WIDE_Y / 100.0
                                         + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
        break;
    case NARROW_FORMAT:
        mCorrectionX = static_cast<long>(static_cast<double>(mCorrectionX)
                                         * static_cast<double>(-DEFAULT_WIDTH)
                                         / ANGLE_NARROW_X / 100.0
                                         + static_cast<double>(DEFAULT_WIDTH) / 2.0);
        mCorrectionY = static_cast<long>(static_cast<double>(mCorrectionY)
                                         * static_cast<double>(DEFAULT_HEIGHT)
                                         / ANGLE_NARROW_Y / 100.0
                                         + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
        break;
    case THERMAL:
        mCorrectionX = static_cast<long>(static_cast<double>(mCorrectionX)
                                         * static_cast<double>(-DEFAULT_WIDTH)
                                         / ANGLE_THERMAL_X / 100.0
                                         + static_cast<double>(DEFAULT_WIDTH) / 2.0);
        mCorrectionY = static_cast<long>(static_cast<double>(mCorrectionY)
                                         * static_cast<double>(DEFAULT_HEIGHT)
                                         / ANGLE_THERMAL_Y / 100.0
                                         + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
        break;
    case ZOOM_THERMAL:
        mCorrectionX = static_cast<long>(static_cast<double>(mCorrectionX)
                                         * static_cast<double>(-DEFAULT_WIDTH)
                                         / ANGLE_ZOOM_THERMAL_X / 100.0
                                         + static_cast<double>(DEFAULT_WIDTH) / 2.0);
        mCorrectionY = static_cast<long>(static_cast<double>(mCorrectionY)
                                         * static_cast<double>(DEFAULT_HEIGHT)
                                         / ANGLE_ZOOM_THERMAL_Y / 100.0
                                         + static_cast<double>(DEFAULT_HEIGHT) / 2.0);
        break;
    }
    mMutex.unlock();
}

/*установка позиции по горизонтали*/
void OptWidget::setPositionEncoderX(long value)
{
    mMutex.lock();
    mPositionEncoderX = value;
    mMutex.unlock();
}

/*установка позиции по вертикали*/
void OptWidget::setPositionEncoderY(long value)
{
    mMutex.lock();
    mPositionEncoderY = value;
    mMutex.unlock();
}

/*установка флага видимости рамки сопровождения*/
void OptWidget::setVisibleTracking(bool state)
{
    mMutex.lock();
    isVisibleTracking = state;
    mMutex.unlock();
}

/*нормализация координат рамки сопровождения*/
QRect OptWidget::norm(QRect rect)
{
    QRect norm_rect = rect;
    int x1, x2, y1, y2;
    int norm_x1, norm_x2, norm_y1, norm_y2;
    norm_rect.getCoords(&x1, &y1, &x2, &y2);
    /* X */
    if (x2 < x1) {
        norm_x2 = x1;
        norm_x1 = x2;
    } else {
        norm_x2 = x2;
        norm_x1 = x1;
    }
    /* Y */
    if (y2 < y1) {
        norm_y2 = y1;
        norm_y1 = y2;
    } else {
        norm_y2 = y2;
        norm_y1 = y1;
    }
    norm_rect.setCoords(norm_x1, norm_y1, norm_x2, norm_y2);
    return norm_rect;
}

/*установка флага отображения ростовой шкалы дальномера*/
void OptWidget::setPixmapOptEnabled()
{
    if (need_to_show_pixmap_opt)
        need_to_show_pixmap_opt = false;
    else
        need_to_show_pixmap_opt = true;
}
