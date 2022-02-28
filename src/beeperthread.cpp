#include "beeperthread.h"

/*инициализация параметров звукового сигнала*/
void BeeperThread::init(snd_pcm_t *pcm_handle, snd_pcm_uframes_t period_size,
                        BeepState beep_state)
{
    this->pcm_handle = pcm_handle;
    this->period_size = period_size;
    this->beep_state = beep_state;
}

/*запуск потока воспроизведения звукового сигнала*/
void BeeperThread::run()
{
    play();
}

/*проигрывание звукового сигнала*/
void BeeperThread::play()
{
    unsigned char *data = (unsigned char *)malloc(period_size);
    int frames = period_size >> 2;
    short s1;
    short s2;
    for(int l1 = 0; l1 < 50; l1++) {
        for(int l2 = 0; l2 < frames; l2++) {
            s1 = s2 = sin(l2 * beep_state.rate) * beep_state.volume;
            data[4 * l2]     = (unsigned char)s1;
            data[4 * l2 + 1] = s1 >> 8;
            data[4 * l2 + 2] = (unsigned char)s2;
            data[4 * l2 + 3] = s2 >> 8;
        }
        while (snd_pcm_writei(pcm_handle, data, frames) < 0) {
            snd_pcm_prepare(pcm_handle);
        }
    }
    free(data);
}


