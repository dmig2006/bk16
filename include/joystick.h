#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>

#include <string>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#include "view.enum.h"
#include "cfgmanager.h"

#define MAX_AXES     16
#define MAX_BUTTONS  16

#define KEY_CAMERA_NARROW_FORMAT  0
#define KEY_CAMERA_WIDE_FORMAT    1
#define KEY_CAMERA_THERMAL        2
#define KEY_CAMERA_ZOOM_THERMAL   3
#define KEY_FIRE                  5
#define KEY_RANGE                 4

struct JoyKeyState {
    bool keyCameraNarrow;
    bool keyCameraWide;
    bool keyCameraThermal;
    bool keyFire;
    bool keyRange;
};

struct JoystickData {
    unsigned char axis_count;
    unsigned char button_count;
    int fd;
    int version;
    char dev_name[80];
    long a_pos[MAX_AXES];
    long b_pos[MAX_BUTTONS];
    bool is_changed;
    js_event event;
};

class Joystick : public QObject
{
    Q_OBJECT

    JoystickData joy_data;
    bool is_opened;
    bool is_blocked;
    bool is_load_enabled;
    long pos_x;
    long pos_y;
    long max_pos;
    CameraMode camera_mode;
    int axis_x_id;
    int axis_y_id;
    JoystickCfg *joystick_cfg;

public:
    Joystick();
    ~Joystick();
    bool open();
    void close();
    void updateEvent();
    void clearLastEvent();
    void setLoadEnabling(bool enable);
    bool getKeyPress(int &key);
    bool getKeyRelease(int &key);
    long getPosX();
    long getPosY();
    long getMaxPos() { return max_pos; }
    bool isOpen() { return is_opened; }
    bool isChange() { return joy_data.is_changed; }

signals:
    void newLogMsg(QString string);
};

#endif // JOYSTICK_H
