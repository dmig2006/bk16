#include "automodetype_dialog.h"
#include "ui_automodetype_dialog.h"

/*Конструктор класса*/
AutoModeTypeDialog::AutoModeTypeDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::AutoModeTypeDialog)
{
    ui->setupUi(this);
}

/*Деструктор класса*/
AutoModeTypeDialog::~AutoModeTypeDialog()
{
    delete ui;
}

/*получение текущего типа атоматического режима*/
void AutoModeTypeDialog::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
    switch (mControllerManager->getAutoType()) {
    case AUTO_TYPE_ORDER:
        ui->inSavingOrderRadioButton->setChecked(true);
        break;
    case AUTO_TYPE_COORDINATE:
        ui->byHorizontalCoordinateRadioButton->setChecked(true);
        break;
    }
}

void AutoModeTypeDialog::colorInversia(bool pFlag)
{
    if (pFlag)
    {
        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

        ui -> byHorizontalCoordinateRadioButton -> setStyleSheet(COLOR_RADIO_BUTTON_NORMA_HORIZONTAL);
        ui -> inSavingOrderRadioButton -> setStyleSheet(COLOR_RADIO_BUTTON_NORMA_SAVE);
    }
    else
    {
        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);

        ui -> byHorizontalCoordinateRadioButton -> setStyleSheet(COLOR_RADIO_BUTTON_INVERSIA_HORIZONTAL);
        ui -> inSavingOrderRadioButton -> setStyleSheet(COLOR_RADIO_BUTTON_INVERSIA_SAVE);
    }
}

/*переопределение обработчика событий изменения состояния*/
void AutoModeTypeDialog::changeEvent(QEvent *event)
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
void AutoModeTypeDialog::closeEvent(QCloseEvent *event)
{
    mControllerManager->setPause(false);
    event->accept();
}

/*обработчик кнопки применить*/
void AutoModeTypeDialog::on_applyButton_clicked()
{
    if (ui->inSavingOrderRadioButton->isChecked())
        mControllerManager->setAutoType(AUTO_TYPE_ORDER);
    if (ui->byHorizontalCoordinateRadioButton->isChecked())
        mControllerManager->setAutoType(AUTO_TYPE_COORDINATE);
    mControllerManager->setPause(false);
    close();
}

/*обработчик кнопки отменить*/
void AutoModeTypeDialog::on_cancelButton_clicked()
{
    mControllerManager->setPause(false);
    close();
}
