#include "soundsignal_dialog.h"
#include "ui_soundsignal_dialog.h"

SoundSignalDialog::SoundSignalDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SoundSignalDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(tr("Off"));
    mnewBeepState.rate = 0.4425f;
    mnewBeepState.volume = (20000 / 100) * 50;
    mnewBeepState.interval = 1;
    mnewBeepState.period = 3;
    mnewBeepState.is_beep_enabled = false;
    mnewBeepState.is_battery_beep_enabled = true;
}

SoundSignalDialog::~SoundSignalDialog()
{
    delete ui;
}

void SoundSignalDialog::changeEvent(QEvent *event)
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

void SoundSignalDialog::closeEvent(QCloseEvent *)
{
    emit closed();
}

void SoundSignalDialog::setControllerManager(ControllerManager* pControllerManager)
{
    mControllerManager = pControllerManager;
}

void SoundSignalDialog::colorInversia(bool pFlag)
{
    if (pFlag)
    {
        ui -> period_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> frequency_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> duration_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> volume_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> alert_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> battaries_label -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_period_measurement_unit -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> label_frequency_measurement_unit -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> label_duration_measurement_unit -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> label_volume_measurement_unit -> setStyleSheet(COLOR_LABEL_NORMA_DATA);

        ui -> horizontalSlider -> setStyleSheet(COLOR_SLIDER_NORMA);
        ui -> horizontalSlider_2 -> setStyleSheet(COLOR_SLIDER_NORMA);
        ui -> horizontalSlider_3 -> setStyleSheet(COLOR_SLIDER_NORMA);
        ui -> horizontalSlider_4 -> setStyleSheet(COLOR_SLIDER_NORMA);

        ui -> apply_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancel_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> alert_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> battaries_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> test_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

        ui -> lineEdit -> setStyleSheet(COLOR_EDIT_SOUND);
        ui -> lineEdit_2 -> setStyleSheet(COLOR_EDIT_SOUND);

    }
    else
    {

        ui -> period_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> frequency_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> duration_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> volume_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> alert_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> battaries_label -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_period_measurement_unit -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> label_frequency_measurement_unit -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> label_duration_measurement_unit -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> label_volume_measurement_unit -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);

        ui -> horizontalSlider -> setStyleSheet(COLOR_SLIDER_INVERSIA);
        ui -> horizontalSlider_2 -> setStyleSheet(COLOR_SLIDER_INVERSIA);
        ui -> horizontalSlider_3 -> setStyleSheet(COLOR_SLIDER_INVERSIA);
        ui -> horizontalSlider_4 -> setStyleSheet(COLOR_SLIDER_INVERSIA);

        ui -> apply_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancel_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> alert_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> battaries_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> test_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

        ui -> lineEdit -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> lineEdit_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> apply_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancel_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> alert_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> battaries_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> test_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
    }
}

void SoundSignalDialog::on_test_Button_clicked()
{
    emit testPlay(mnewBeepState);
}

void SoundSignalDialog::on_horizontalSlider_2_sliderMoved(int position)
{
    QString str;
    if (position >= 4700)
        mnewBeepState.rate = 0.675;
    else if (position >= 3100)
        mnewBeepState.rate = 0.4425;
    else if (position >= 2500)
        mnewBeepState.rate = 0.355;
    else if (position >= 700)
        mnewBeepState.rate = 0.0998;
    str.setNum(position);
    ui->label_frequency_measurement_unit->setText(str + tr("Hz"));
}

void SoundSignalDialog::on_horizontalSlider_4_sliderMoved(int position)
{
    mnewBeepState.volume = (20000 / 100) * position;
    QString str;
    str.setNum(position);
    ui->label_volume_measurement_unit->setText(str + "%");
}

void SoundSignalDialog::on_horizontalSlider_3_sliderMoved(int position)
{
    QString str;
    str.setNum(position / 1000);
    mnewBeepState.interval = position / 1000;
    ui->label_duration_measurement_unit->setText(str + tr("s"));
}

void SoundSignalDialog::on_horizontalSlider_sliderMoved(int position)
{
    mnewBeepState.period = (int)(position / 1000);
    QString str;
    str.setNum((int)(position / 1000));
    ui->label_period_measurement_unit->setText(str + tr(" min"));
}

void SoundSignalDialog::on_alert_Button_clicked()
{
    if (mnewBeepState.is_beep_enabled)
    {
        mnewBeepState.is_beep_enabled = false;
        ui->lineEdit->setText(tr("Off"));
    }
    else
    {
        mnewBeepState.is_beep_enabled = true;
        ui->lineEdit->setText(tr("On"));
    }
}

void SoundSignalDialog::on_apply_Button_clicked()
{
    emit newBeepState(mnewBeepState);
    this->close();
}

void SoundSignalDialog::on_cancel_Button_clicked()
{
    this->close();
}

void SoundSignalDialog::on_battaries_Button_clicked()
{
    if (mnewBeepState.is_battery_beep_enabled)
    {
        mnewBeepState.is_battery_beep_enabled = false;
        ui->lineEdit_2->setText(tr("Off"));
    }
    else
    {
        mnewBeepState.is_battery_beep_enabled = true;
        ui->lineEdit_2->setText(tr("On"));
    }
}
