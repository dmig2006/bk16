#ifndef AUTOMODETYPE_H
#define AUTOMODETYPE_H

#include <QDialog>
#include <QCloseEvent>
#include "controllermanager.h"			// Controller

namespace Ui {
    class AutoModeTypeDialog;
}

class AutoModeTypeDialog : public QDialog
{
    Q_OBJECT

    Ui::AutoModeTypeDialog *ui;
    ControllerManager *mControllerManager;

public:
    AutoModeTypeDialog(QWidget *parent = 0);
    ~AutoModeTypeDialog();
    void setControllerManager(ControllerManager *pControllerManager);
    void colorInversia(bool pFlag);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // AUTOMODETYPE_H
