#ifndef AMMUNITION_H
#define AMMUNITION_H

#include <QDialog>
#include <QCloseEvent>
#include <QIntValidator>

#include "controllermanager.h"
#include "weapon.enum.h"

namespace Ui {
    class AmmunitionDialog;
}

class AmmunitionDialog : public QDialog
{
    Q_OBJECT

    Ui::AmmunitionDialog *ui;
    ControllerManager* mControllerManager;

public:
    AmmunitionDialog(QWidget *parent = 0, WeaponType w_type = WEAPON_KORD);
    ~AmmunitionDialog();
    void setControllerManager(ControllerManager* pControllerManager);
    void colorInversia(bool pFlag);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();
};

#endif // AMMUNITION_H
