#ifndef BURSTLENGTH_H
#define BURSTLENGTH_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>
#include <QIntValidator>

#include "controllermanager.h"			// Controller

namespace Ui {
    class BurstLengthDialog;
}

class BurstLengthDialog : public QDialog
{
    Q_OBJECT

    Ui::BurstLengthDialog *ui;
    ControllerManager *mControllerManager;

public:
    BurstLengthDialog(QWidget *parent = 0);
    ~BurstLengthDialog();
    void setControllerManager(ControllerManager *pControllerManager);
    void colorInversia(bool pFlag);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // BURSTLENGTH_H
