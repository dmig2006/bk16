#include "beeper.h"

/*Конструктор класса*/
Beeper::Beeper(QObject *parent) :
    QThread(parent)
{
    is_device_ready = false;
    is_device_ready = openDevice();
    mBeepState.rate = 0.4425f;
    mBeepState.volume = (20000 / 100) * 50;
    mBeepState.interval = 1;
    mBeepState.period = 3;
    mBeepStateBattery.rate     = 0.4125f;
    mBeepStateBattery.volume   = 20000;
    mBeepStateBattery.interval = BATTERY_COUNT_TIME;
    mBeepStateBattery.period   = BATTERY_BEEP_PERIOD;
    isBeep = false;
    mBeepState.is_beep_enabled = false;
    mBeepState.is_battery_beep_enabled = true;
    battery_timer_id = -1;
    signal_timer_id  = -1;
    low_battery      = false;
    periodic_signal  = false;
    battery_beep     = false;
    signal_beep      = false;
    battery_timer_id = startTimer(BATTERY_BEEP_PERIOD * 60 * 1000);
    need_exit = false;
}

/*Деструктор класса*/
Beeper::~Beeper()
{
    need_exit = true;
    msleep(200);
    closeDevice();
    wait(1000);
}

/*установка параметров звукового сигнала*/
void Beeper::setBeepState(BeepState pBeepState)
{
    mBeepState = pBeepState;
    mBeepStateBattery.interval = pBeepState.interval;
}

/*сброс периодического сигнала*/
void Beeper::resetBeep()
{
    mMutex.lock();
    isBeep = false;
    mMutex.unlock();
}

/*проигрывание по низкому напряжению*/
void Beeper::beepBatteryPlay()
{
    mMutex.lock();
    if (is_device_ready) {
        if (mBeepState.is_battery_beep_enabled)
            play(mBeepStateBattery);
    } else {
        emit newLogMsg(tr("Play Sound Error"));
    }
    mMutex.unlock();
}

/*проигрывание по кнопке тест*/
void Beeper::testPlay(BeepState pBeepState)
{
    if (!is_device_ready)
        return;
    BeeperThread *beeper_thread = new BeeperThread(this);
    beeper_thread->init(pcm_handle, periodsize, pBeepState);
    beeper_thread->start();
}

/*установка флага вкл/выкл сигнала низкого напряжения*/
void Beeper::setBatteryBeepFlag(bool enabled, bool first)
{
    low_battery = enabled;
    if (first)
        battery_beep = true;
}

/*установка флага вкл/выкл и таймера периодического сигнала*/
void Beeper::setBeepSignal(bool enabled)
{
    if (periodic_signal == enabled)
        return;
    periodic_signal = enabled;
    if (enabled) {
        emit newLogMsg(tr("Periodic sound signal: on"));
        signal_timer_id = startTimer(mBeepState.period * 60 * 1000);
    } else {
        emit newLogMsg(tr("Periodic sound signal: off"));
        killTimer(signal_timer_id);
        signal_timer_id = -1;
    }
}

/*цикл воспроизведекния*/
void Beeper::run()
{
    while (!need_exit) {
        msleep(1500);
        if (is_device_ready) {
            if (periodic_signal && signal_beep) {
                play(mBeepState);
                signal_beep = false;
            }
            if (low_battery && battery_beep
                    && mBeepState.is_battery_beep_enabled) {
                play(mBeepStateBattery);
                msleep(500);
                play(mBeepStateBattery);
                battery_beep = false;
            }
        } else {
            emit newLogMsg(tr("Play Sound Error"));
        }
    }
}

/*обработчик таймеров на сигналы низкого напряжения и периодический*/
void Beeper::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == battery_timer_id) {
        resetBatteryBeepFlag();
        return;
    }
    if (event->timerId() == signal_timer_id) {
        resetSignalBeepFlag();
        return;
    }
}

/*открытие устройства воспроизведения звукового сигнала*/
bool Beeper::openDevice()
{
    if (is_device_ready)
        return true;
    pcm_name = strdup("plughw:0,0");
    stream = SND_PCM_STREAM_PLAYBACK;
    rate = 44100;
    periods = 2;
    periodsize = 2048;
    snd_pcm_hw_params_alloca(&hwparams);
    err = snd_pcm_open(&pcm_handle, pcm_name, stream, 0);
    if (err < 0) {
        emit newLogMsg("Error opening PCM device [ "
                       + QString(pcm_name) + " ]: "
                       + QString(snd_strerror(err)));
        return false;
    }
    err = snd_pcm_hw_params_any(pcm_handle, hwparams);
    if (err < 0) {
        emit newLogMsg("Can not configure this PCM device: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    err = snd_pcm_hw_params_set_access(pcm_handle, hwparams,
                                       SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        emit newLogMsg("Error setting access: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    err = snd_pcm_hw_params_set_format(pcm_handle, hwparams,
                                       SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        emit newLogMsg("Error setting format: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    exact_rate = rate;
    err = snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, 0);
    if (err < 0) {
        emit newLogMsg("Error setting rate: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    if (rate != exact_rate) {
        emit newLogMsg("The rate " + QString::number(rate) +
                       " Hz is not supported by your hardware.\n ==> Using "
                       + QString::number(exact_rate) + " Hz instead.\n");
    }
    err = snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 2);
    if (err < 0) {
        emit newLogMsg("Error setting channels: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    err = snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0);
    if (err < 0) {
        emit newLogMsg("Error setting periods: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    //# set buffer size (in frames). The resulting latency is given by #
    //# latency = periodsize * periods / (rate * bytes_per_frame)      #
    err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams,
                                            (periodsize * periods) >> 2);
    if (err < 0) {
        emit newLogMsg("Error setting buffersize: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    err = snd_pcm_hw_params(pcm_handle, hwparams);
    if (err < 0) {
        emit newLogMsg("Error setting HW params: "
                       + QString(snd_strerror(err)) + "\n");
        return false;
    }
    return true;
}

/*закрытие устройства воспроизведения звукового сигнала*/
void Beeper::closeDevice()
{
    snd_pcm_drop(pcm_handle);
    snd_pcm_close(pcm_handle);
}

/*воспроизведение звукового сигнала*/
void Beeper::play(BeepState beep_state)
{
    data = (unsigned char*)malloc(periodsize);
    frames = periodsize >> 2;
    for (l1 = 0; l1 < 50 * beep_state.interval; ++l1) {
        for (l2 = 0; l2 < frames; ++l2) {
            s1 = s2 = sin(l2 * beep_state.rate) * beep_state.volume;
            data[4 * l2] = (unsigned char)s1;
            data[4 * l2 + 1] = s1 >> 8;
            data[4 * l2 + 2] = (unsigned char)s2;
            data[4 * l2 + 3] = s2 >> 8;
        }
        while ((pcmreturn = snd_pcm_writei(pcm_handle, data, frames)) < 0)
            snd_pcm_prepare(pcm_handle);
    }
    free(data);
}

/*установка флага сигнала низкого напряжения по таймеру*/
void Beeper::resetBatteryBeepFlag()
{
    if (!battery_beep)
        battery_beep = true;
}

/*установка флага периодического сигнала по таймеру*/
void Beeper::resetSignalBeepFlag()
{
    if (!signal_beep)
        signal_beep = true;
}
