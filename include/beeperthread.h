#ifndef BEEPER_THREAD_H
#define BEEPER_THREAD_H

#include <QThread>
#include <cmath>
#include <cstdio>
#include <alsa/asoundlib.h>

struct BeepState {
    int interval;
    int period;
    int volume;
    int is_beep_enabled;
    int is_battery_beep_enabled;
    float rate;
};

class BeeperThread : public QThread
{
    Q_OBJECT

    snd_pcm_t *pcm_handle;
    snd_pcm_uframes_t period_size;
    BeepState beep_state;

    void play();

protected:
    void run();

public:
    explicit BeeperThread(QObject *parent = 0): QThread(parent) {}
    void init(snd_pcm_t *pcm_handle, snd_pcm_uframes_t period_size,
              BeepState beep_state);
};

#endif // BEEPER_THREAD_H
