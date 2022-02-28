#ifndef ADJUSTMENT_H
#define ADJUSTMENT_H

#include <QDialog>
#include <QCloseEvent>
#include <QThread>
#include <QMutex>

#include "controllermanager.h"

namespace Ui {
    class AdjustmentDialog;
}

class AdjustmentDialog : public QDialog
{
    Q_OBJECT

    ControllerManager *mControllerManager;
    Ui::AdjustmentDialog *ui;
    long mPosX;
    long mPosY;

public:
    AdjustmentDialog(QWidget *parent = 0);
    ~AdjustmentDialog();
    void setControllerManager(ControllerManager *pControllerManager);
    void colorInversia(bool pInversia);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_upButton_pressed();
    void on_leftButton_pressed();
    void on_downButton_pressed();
    void on_rightButton_pressed();
    void on_centerButton_clicked();
    void on_applyButton_clicked() {}
    void on_applyButton_released();
    void on_cancelButton_clicked();
};

#endif // ADJUSTMENT_H
