#ifndef PLATHORMDIR_H
#define PLATHORMDIR_H

#include <QWidget>
#include <QImage>
#include <QMutex>
#include <QPainter>
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

namespace Ui {
    class PlathormDir;
}

class PlathormDir : public QWidget
{
    Q_OBJECT

public:
    explicit PlathormDir(QWidget *parent = 0);
    ~PlathormDir();

    void setAngle(int pAngle);
    void inversia(bool pFlag);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::PlathormDir *ui;
    QMutex mMutex;
    int mAngle;
    QImage mPixmapDir;
    QImage mPixmapBackground;
};

#endif // PLATHORMDIR_H
