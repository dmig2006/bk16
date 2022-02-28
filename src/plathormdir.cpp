#include "plathormdir.h"
#include "ui_plathormdir.h"

/*Конструктор класса*/
PlathormDir::PlathormDir(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlathormDir)
{
    ui->setupUi(this);
    mAngle = 0;
    mPixmapDir.load("://res/images/TOP MBDU 75.png");
}

/*Деструктор класса*/
PlathormDir::~PlathormDir()
{
    delete ui;
}

/*устанновка позиции по горизонтали*/
void PlathormDir::setAngle(int pAngle)
{
    mAngle = pAngle;
    update();
}

void PlathormDir::inversia(bool pFlag)
{
    if (pFlag)
    {
        mPixmapDir.load(":/res/images/TOP MBDU 75.png");
        mPixmapBackground.load(":/compass.png");

    }
    else
    {
        mPixmapDir.load(":/res/images/compass_taifun_inversia.png");
        mPixmapBackground.load(":/res/images/compass_invers.png");
    }
}

/*отрисовка позиции по горизонтали*/
void PlathormDir::paintEvent(QPaintEvent *)
{
    QPainter pb(this);
    pb.drawImage(width() / 2 - mPixmapBackground.width() / 2,
                 height() / 2 - mPixmapBackground.height() / 2 - 10,
                 mPixmapBackground);
    pb.translate(width() / 2 - mPixmapDir.width() / 2,
                 height() / 2 - mPixmapDir.height() / 2 - 5);
    pb.translate(mPixmapDir.width() / 2, mPixmapDir.height() / 2 - 5);
    pb.rotate(mAngle);
    pb.translate(-mPixmapDir.width() / 2, -mPixmapDir.height() / 2 - 5);
    pb.drawImage(0, 5, mPixmapDir);
    pb.end();
}
