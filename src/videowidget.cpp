 #include "videowidget.h"
#include "ui_videowidget.h"

/*Конструктор класса*/
VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoWidget)
{
    ui->setupUi(this);
    isOperationMode = true;
    mImage = mZeroImage;
    mCamera = new Camera();
    connect(mCamera, SIGNAL(newFrame(QImage)),   SLOT(setImage(QImage)));
    connect(mCamera, SIGNAL(closeCamera()),      SLOT(closeConnection()));
    connect(mCamera, SIGNAL(newLogMsg(QString)), SIGNAL(newLogMsg(QString)));
    connect(mCamera, SIGNAL(gotConnection()),    SIGNAL(resetCamera()));
    mConnect = true;
    flagInversia = true;
    mCamera->start();
}

/*Деструктор класса*/
VideoWidget::~VideoWidget()
{
    delete mCamera;
    delete ui;
}

/*установка камеры для отображения*/
void VideoWidget::setCameraMode(CameraMode pCameraMode)
{
    mCamera->setCamera(pCameraMode);
}

void VideoWidget::closeConnection()
{
    mConnect = false;
}

void VideoWidget::getFlagInversia(bool pFlag)
{
    flagInversia = pFlag;
}

/*установка кадра*/
void VideoWidget::setImage(QImage image)
{
    mConnect = true;
    mImage = image;
    update();
}

/*отрасовка элементов*/
void VideoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);

    if (isOperationMode)
    {
        if (mConnect)
        {

           painter.drawImage(0, 0, mImage);
        }
        else
        {
            mImage = mZeroImage;
            painter.drawImage(0, 0, mImage);

            if (flagInversia)
            {
                painter.setPen(QColor::fromRgb(0x000000));
            }
            else
            {
                painter.setPen(QColor::fromRgb(0xF68080));
            }

            painter.drawText(width() / 2 - 50, height() / 2, 150, 600, true,
                             tr("LOADING"));
        }
    }
    else
    {
        mImage = mZeroImage;
        painter.drawImage(0, 0, mImage);

        if (flagInversia)
        {
            painter.setPen(QColor::fromRgb(0x000000));
        }
        else
        {
            painter.setPen(QColor::fromRgb(0xF68080));
        }

        if (isConnected)
            painter.drawText(width() / 2 - 50, height() / 2, 150, 600, true,
                             tr("STANDBY MODE"));
        else
        {
            painter.drawText(width() / 2 - 50, height() / 2, 150, 600, true,
                             tr("NOT CONNECTED"));
        }
    }
    painter.end();
}

/*установка дежурного/рабочего режимов*/
void VideoWidget::setOperationMode(bool state, bool isNotConnetcted)
{
    mMutex.lock();
    isOperationMode = state;
    isConnected = !isNotConnetcted;
    mCamera->setOperationMode(state);
    mMutex.unlock();
}
