#ifndef OPTWIDGET_H
#define OPTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPicture>
#include <QImage>
#include <QBitmap>
#include <QMouseEvent>
#include <QList>
#include <QVector>
#include <QPalette>

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "controllermanager.h"
#include "aimlist.h"
#include "view.enum.h"

#define TEXT_COLOR(color, text)	QString("<FONT COLOR=%1>%2</FONT>").arg(color).arg(text)

namespace Ui {
    class OptWidget;
}

struct Graphics {
    QImage pixmapCrossNarrow;
    QImage pixmapCrossWide;
    QImage pixmapCrossThermo;
    QImage pixmapCrossZoomThermo;
    QImage pixmapOptNarrow;
    QImage pixmapOptWide;
    QImage pixmapOptThermo;
    QImage pixmapOptZoomThermo;
    QImage pixmapAim;
    QImage pixmapCrossTg;
    QImage pixmapTriangleHor;
    QImage pixmapTriangleVert;
    QImage pixmapCenter;
    QImage pixmapRectNarrow;
    QImage pixmapOptRange;
    QImage pixmapLogo;
};

class OptWidget : public QWidget
{
    Q_OBJECT

    bool need_to_show_pixmap_opt;

public:
    explicit OptWidget(QWidget *parent = 0);
    ~OptWidget();
    void setControllerManager(ControllerManager *pControllerManager);
    void setJustCross(long pJustX, long pJustY);
    void setCorrectionCross(long pCorrectionX, long pCorrectionY);
    void setTracking(bool state);
    void setTypeTracking(int pType);
    void setStabilization(bool state);
    void setPositionEncoderX(long value);
    void setPositionEncoderY(long value);
    void setCameraMode(CameraMode pCameraMode);
    void optRangeUp();
    void optRangeDown();
    void updateOptRange(int pHeinghtOptRange, CameraMode pCameraMode);
    void setVisibleOptRange(bool state);
    bool getTracking() { return mTracking; }
    int  getTypeTracking() { return mTypeTracking; }
    void setVisibleTracking(bool state);
    bool getFilter() { return isFilter; }
    bool setFilter(bool state);
    void setAimType(AimType pAimType);
    void setRange(int pRange);
    void setPixmapOptEnabled();
    void setNullRect();
    void setOperationMode(bool enabled);
    void setCocking(bool enabled);


    bool flagLabel;

public slots:
    void setCenterMassTracking(int x, int y);
    void setRects(QRect pRect_t1, QRect pRect_t2);
    void invertCrossColor();

    //Zala
        void setBlockPP(bool enabled);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    Ui::OptWidget *ui;
    ControllerManager *mControllerManager;
    CameraMode mCameraMode;
    AimType mAimType;
    Graphics mGraphics;
    Graphics mGraphicsBlack;
    Graphics mGraphicsWhite;
    QImage imageLoading;
    QImage imageStandByMode;
    QImage pixmapCross;
    QImage pixmapCrossTg;
    QImage pixmapCenter;
    QImage pixmapOpt;
    QImage pixmapOptRange;
    QImage pixmapNarrow;
    QImage pixmapL;
    QColor color;
    QMutex mMutex;
    QRect mRect_t1;
    QRect mRect_t2;
    QRect mRectTracking;
    std::vector<QRect> rects;
    std::vector<QRect> mTargets;
    long mPositionEncoderX;
    long mPositionEncoderY;
    long mPositionX;
    long mPositionY;
    long mCorrectionX;
    long mCorrectionY;
    long mJustX;
    long mJustY;
    bool mVisibleOptRange;
    bool mTracking;
    bool mStabilization;
    bool mConnect;
    bool isFilter;
    bool isConnect;
    bool isInvertColor;
    bool isPressRect;
    bool isVisibleTracking;
    bool is_cursor_in_area;
    bool is_operation_mode;
    bool is_cocking;
    int mHeightOptRange;
    int idColor;
    int mRectX;
    int mRectY;
    int mTypeTracking;
    int mRectWidth;
    int mRectHeight;
    int mRange;


    QRect norm(QRect rect);
    void updateRect();

private slots:


signals:
    void move_Ac_Target(long x, long y);

};

#endif // OPTWIDGET_H
