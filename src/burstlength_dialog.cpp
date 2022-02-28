#include "burstlength_dialog.h"
#include "ui_burstlength_dialog.h"

BurstLengthDialog::BurstLengthDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::BurstLengthDialog)
{
    ui->setupUi(this);
    //startTimer(10);

    QIntValidator *short_burst_validator = new QIntValidator(1, 5, this);
    QIntValidator *long_burst_validator = new QIntValidator(6, 50, this);
    QIntValidator *single_shot_time_validator = new QIntValidator(50, 200, this);

    ui->shortBurstLengthInput->setValidator(short_burst_validator);
    ui->longBurstLengthInput->setValidator(long_burst_validator);
    ui->singleShotTimeInput->setValidator(single_shot_time_validator);
}

BurstLengthDialog::~BurstLengthDialog()
{
    delete ui;
}

void BurstLengthDialog::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
    ui->shortBurstLengthInput->setText(QString::number(mControllerManager->getShortBurst()));
    ui->longBurstLengthInput->setText(QString::number(mControllerManager->getLongBurst()));
    ui->singleShotTimeInput->setText(QString::number(mControllerManager->getShotTime()));
}

void BurstLengthDialog::colorInversia(bool pFlag)
{
    if (pFlag)
    {
        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

        ui -> shortBurstLengthLabel -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> shortBurstLengthInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> shortBurstLengthMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);

        ui -> longBurstLengthLabel -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> longBurstLengthInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> longBurstLengthMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);

        ui -> singleShotTimeLabel -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> singleShotTimeInput -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> singleShotTimeMeasurementUnit -> setStyleSheet(COLOR_LABEL_NORMA);

        ui -> line -> setStyleSheet(COLOR_LINE_NORMA);
    }
    else
    {
        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);

        ui -> shortBurstLengthLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> shortBurstLengthInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> shortBurstLengthMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);

        ui -> longBurstLengthLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> longBurstLengthInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> longBurstLengthMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);

        ui -> singleShotTimeLabel -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> singleShotTimeInput -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> singleShotTimeMeasurementUnit -> setStyleSheet(COLOR_LABEL_INVERSIA);

        ui -> line -> setStyleSheet(COLOR_LINE_INVERSIA);
    }
}

void BurstLengthDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void BurstLengthDialog::closeEvent(QCloseEvent *event)
{
     mControllerManager->setPause(false);
     event->accept();
}

void BurstLengthDialog::on_applyButton_clicked()
{
    int short_burst = ui->shortBurstLengthInput->text().toInt();
    int long_burst = ui->longBurstLengthInput->text().toInt();
    int shot_time = ui->singleShotTimeInput->text().toInt();

    mControllerManager->setShortBurst(short_burst);
    mControllerManager->setLongBurst(long_burst);
    mControllerManager->setShotTime(shot_time);

    mControllerManager->setPause(false);
    close();
}

void BurstLengthDialog::on_cancelButton_clicked()
{
    mControllerManager->setPause(false);
    close();
}
