#ifndef BEEPER_H
#define BEEPER_H

#include <QThread>
#include <QMutex>
#include <QString>
#include <QTimerEvent>

#include <iostream>
#include <ctime>

#include <cmath>
#include <alsa/asoundlib.h>

#include "beeperthread.h"

#define BATTERY_BEEP_PERIOD      2      // min
#define BATTERY_COUNT_TIME       2      // sec
#define BATTERY_SIGNAL_FREQUENCY 3100   // Hz

class Beeper : public QThread
{
    Q_OBJECT

public:
    explicit Beeper(QObject *parent = 0);
    ~Beeper();
    void setBeepState(BeepState pBeepState);
    void resetBeep();
    void beepBatteryPlay();
    void testPlay(BeepState pBeepState);
    void setBatteryBeepFlag(bool enabled, bool first);
    void setBeepSignal(bool enabled);

protected:
    void run();
    void timerEvent(QTimerEvent *event);

private:
    bool openDevice();
    void closeDevice();
    void play(BeepState beep_state);
    void resetBatteryBeepFlag();
    void resetSignalBeepFlag();

    int err;
    unsigned int rate;
    int dir;
    int periods;
    int pcmreturn;
    int l1;
    int l2;
    int frames;
    unsigned int exact_rate;
    short s1;
    short s2;
    char *pcm_name;
    unsigned char *data;
    bool is_device_ready;
    snd_pcm_t *pcm_handle;
    snd_pcm_stream_t stream;
    snd_pcm_hw_params_t *hwparams;
    snd_pcm_uframes_t periodsize;
    bool isBeep;
    BeepState mBeepState;
    BeepState mBeepStateBattery;
    QMutex mMutex;
    int battery_timer_id;
    int signal_timer_id;
    bool low_battery;
    bool periodic_signal;
    bool battery_beep;
    bool signal_beep;
    bool need_exit;

signals:
    void newLogMsg(QString str);
};

#endif // BEEPER_H
