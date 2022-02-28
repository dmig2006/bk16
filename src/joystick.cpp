#include "joystick.h"

/*Коструктор класса*/
Joystick::Joystick()
    : is_opened(false), is_blocked(false), is_load_enabled(false),
      pos_x(0), pos_y(0)
{
    joystick_cfg = CfgManager::getSingletonPtr()->getJoystickCfg();
    axis_x_id = joystick_cfg->mAxisX;
    axis_y_id = joystick_cfg->mAxisY;
    max_pos = joystick_cfg->mMaxXright;
}

/*Деструктор класса*/
Joystick::~Joystick()
{
    close();
}

/*открытие устройства*/
bool Joystick::open()
{
    joy_data.fd = ::open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
    if (joy_data.fd == -1) {
        is_opened = false;
        emit newLogMsg(tr("Joystick not connected"));
        return false;
    }
    // get pad info ...
    ioctl(joy_data.fd, JSIOCGAXES,    &joy_data.axis_count);
    ioctl(joy_data.fd, JSIOCGBUTTONS, &joy_data.button_count);
    ioctl(joy_data.fd, JSIOCGVERSION, &joy_data.version);
    ioctl(joy_data.fd, JSIOCGNAME(sizeof(joy_data.dev_name)),
          &joy_data.dev_name);

    // set default values
    joy_data.is_changed = false;
    memset(&joy_data.a_pos, 0, sizeof(joy_data.a_pos));
    memset(&joy_data.b_pos, 0, sizeof(joy_data.b_pos));
    is_opened = true;
    emit newLogMsg(tr("Joystick connected"));
    return true;
}

/*закрытие устройства*/
void Joystick::close()
{
    ::close(joy_data.fd);
    is_opened = false;
}

/*обработка событий на перемещение от джойстика*/
void Joystick::updateEvent()
{
    if (!is_opened) {
        if (!is_blocked && !is_load_enabled) {
            pos_x = 0;
            pos_y = 0;
            open();
            sleep(2);
        }
        return;
    }

    memset(&joy_data.event, 0, sizeof(joy_data.event));
    int ret = read(joy_data.fd, &joy_data.event, sizeof(joy_data.event));
    if (ret > 0 || (ret == -1 && errno == EAGAIN)) {
        switch (joy_data.event.type) {
        case JS_EVENT_INIT:
        case JS_EVENT_INIT | JS_EVENT_AXIS:
        case JS_EVENT_INIT | JS_EVENT_BUTTON:
            break;
        case JS_EVENT_AXIS:
            joy_data.a_pos[joy_data.event.number] = joy_data.event.value;
            joy_data.is_changed = true;
            break;
        case JS_EVENT_BUTTON:
            joy_data.b_pos[joy_data.event.number] = joy_data.event.value;
            joy_data.is_changed = true;
            break;
        }
    } else {
        pos_x = 0;
        pos_y = 0;
        close();
        emit newLogMsg(tr("Joystick not connected"));
        return;
    }

    if (joy_data.is_changed) {
        pos_x = joy_data.a_pos[axis_x_id];
        pos_y = joy_data.a_pos[axis_y_id];
        joy_data.is_changed = false;
    }
}

/*очистка данных*/
void Joystick::clearLastEvent()
{
    memset(&joy_data.event, 0, sizeof(joy_data.event));
}

/*установка флага на взвод*/
void Joystick::setLoadEnabling(bool enable)
{
    is_load_enabled = enable;
}

/*получение номера нажатой кнопки*/
bool Joystick::getKeyPress(int &key)
{
    bool state = false;
    for (int i = 0; i < 14; ++i) {
        int v = joy_data.b_pos[i];
        if (v != 0) {
            key = i;
            state = true;
            break;
        }
    }
    return state;
}

/*получение номера отпущеной кнопки*/
bool Joystick::getKeyRelease(int &key)
{
    bool state = false;
    for (int i = 0; i < 14; ++i) {
        int v = joy_data.b_pos[i];
        if (v == 0) {
            key = i;
            state = true;
            break;
        }
    }
    return state;
}

/*получение позиции по горизонтали*/
long Joystick::getPosX()
{
    if (pos_x > 32767)
        pos_x = 32767;
    if (pos_x < -32767)
        pos_x = -32767;
    return pos_x;
}

/*получение позиции по вертикали*/
long Joystick::getPosY()
{
    if (pos_y > 32767)
        pos_y = 32767;
    if (pos_y < -32767)
        pos_y = -32767;
    return pos_y;
}
