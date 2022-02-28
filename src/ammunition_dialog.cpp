#include "ammunition_dialog.h"
#include "ui_ammunition_dialog.h"

/*Конструктор класса*/
AmmunitionDialog::AmmunitionDialog(QWidget *parent, WeaponType w_type) :
    QDialog(parent), ui(new Ui::AmmunitionDialog)
{
    ui->setupUi(this);
    QIntValidator *validator;
    if (w_type == WEAPON_AGS || w_type == WEAPON_AG40)
        validator = new QIntValidator(0, 100, this);
    else
        validator = new QIntValidator(0, 1000, this);
    ui->ammunitionInput->setValidator(validator);
}

/*Деструктор класса*/
AmmunitionDialog::~AmmunitionDialog()
{
    delete ui;
}

/*получение количества амуниции*/
void AmmunitionDialog::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
    ui->ammunitionInput->setText(QString::number(mControllerManager->getAmmunitionCounter()));
}

void AmmunitionDialog::colorInversia(bool pFlag)
{
    if (pFlag)
    {
        ui -> ammunitionInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> ammunitionMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);

        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
    }
    else
    {
        ui -> ammunitionInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> ammunitionMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);

        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
    }
}

/*переопределение обработчика событий изменения состояния*/
void AmmunitionDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*обработчик события closeEvent*/
void AmmunitionDialog::closeEvent(QCloseEvent *event)
{
    mControllerManager->setPause(false);
    event->accept();
}

/*обработчик кнопки применить*/
void AmmunitionDialog::on_applyButton_clicked()
{
    long lValue = ui->ammunitionInput->text().toLong();
    if (lValue > 0 && lValue < 100000)
        mControllerManager->setAmmunitionCounter(lValue, true);
    mControllerManager->setPause(false);
    close();
}

/*обработчик кнопки отменить*/
void AmmunitionDialog::on_cancelButton_clicked()
{
    mControllerManager->setPause(false);
    close();
}
