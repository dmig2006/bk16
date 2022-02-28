#ifndef SOUNDSIGNAL_H
#define SOUNDSIGNAL_H

#include <QDialog>

#include "beeper.h"
#include "controllermanager.h"

namespace Ui {
    class SoundSignalDialog;
}

class SoundSignalDialog : public QDialog
{
    Q_OBJECT

    Ui::SoundSignalDialog *ui;
    ControllerManager *mControllerManager;
    BeepState mnewBeepState;
    QString header;

public:
    SoundSignalDialog(QWidget *parent = 0);
    ~SoundSignalDialog();
    void setControllerManager(ControllerManager *pControllerManager);
    void colorInversia(bool pFlag);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *);

private slots:
    void on_battaries_Button_clicked();
    void on_cancel_Button_clicked();
    void on_apply_Button_clicked();
    void on_alert_Button_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_horizontalSlider_3_sliderMoved(int position);
    void on_horizontalSlider_4_sliderMoved(int position);
    void on_horizontalSlider_2_sliderMoved(int position);
    void on_test_Button_clicked();

signals:
    void newLog(QString str);
    void testPlay(BeepState pBeepState);
    void newBeepState(BeepState);
    void closed();
};

#endif // SOUNDSIGNAL_H
