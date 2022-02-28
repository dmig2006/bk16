#ifndef ACTION_H
#define ACTION_H

#define ACTION_NOVALID                          0x00
#define ACTION_TYPE_SPEED_X                     0x53
#define ACTION_TYPE_SPEED_Y                     0x11
#define ACTION_TYPE_POSITION_X                  0x12
#define ACTION_TYPE_POSITION_Y                  0x54
#define ACTION_TYPE_GET_TRMO_0                  0x14
#define ACTION_TYPE_GET_TRMO_1                  0x15
#define ACTION_TYPE_TRMO_0                      0x16
#define ACTION_TYPE_TRMO_1                      0x17
#define ACTION_TYPE_GET_PRESSURE                0x18
#define ACTION_TYPE_PRESSURE                    0x19
#define ACTION_TYPE_GET_AMMUNITION_COUNTER      0x20
#define ACTION_TYPE_AMMUNITION_COUNTER          0x21
#define ACTION_TYPE_BOLT_NOTCH                  0x22
#define ACTION_TYPE_GET_ARMAMENT                0x23
#define ACTION_TYPE_ARMAMENT                    0x24
#define ACTION_TYPE_GET_NUMBER_ADC              0x25
#define ACTION_TYPE_NUMBER_ADC                  0x26
#define ACTION_TYPE_GET_ENCODER_SPEED_X         0x30
#define ACTION_TYPE_GET_ENCODER_SPEED_Y         0x31
#define ACTION_TYPE_ENCODER_SPEED_X             0x32
#define ACTION_TYPE_ENCODER_SPEED_Y             0x33
#define ACTION_TYPE_GET_ENCODER_POSITION_X      0x34
#define ACTION_TYPE_GET_ENCODER_POSITION_Y      0x35
#define ACTION_TYPE_ENCODER_POSITION_X          0x36
#define ACTION_TYPE_ENCODER_POSITION_Y          0x37
#define ACTION_TYPE_INIT_AMMUNITION_COUNTER     0x38
#define ACTION_TYPE_GET_RANGE                   0x39
#define ACTION_TYPE_RANGE                       0x40
#define ACTION_TYPE_ACHIEVEMENT_POSITION        0x41
#define ACTION_TYPE_CLOSE                       0x42
#define ACTION_TYPE_GET_VOLTAGE_BATTERY         0x27
#define ACTION_TYPE_VOLTAGE_BATTERY             0x28
#define ACTION_TYPE_GET_VOLTAGE_EXTERNAL        0x43
#define ACTION_TYPE_VOLTAGE_EXTERNAL            0x44
#define ACTION_TYPE_STAND_BY_MODE               0x45
#define ACTION_TYPE_OPERATION_MODE              0x46
#define ACTION_TYPE_BURST_SHORT                 0x47
#define ACTION_TYPE_BURST_LONG                  0x48
#define ACTION_TYPE_FIRE_TYPE                   0x49
#define ACTION_TYPE_FIRE                        0x29
#define ACTION_TYPE_STABILIZATION               0x50
#define ACTION_TYPE_SET_VELOCITY_ENABLE         0x51
#define ACTION_TYPE_SET_VELOCITY_DISABLE        0x55
#define ACTION_TYPE_SET_BRAKE                   0x52
#define ACTION_TYPE_SWITCH_STABILIZATION        0x56
#define ACTION_TYPE_SWITCH_WIPER                0x70
#define ACTION_TYPE_RESET_JOYSTICK              0x60
#define ACTION_TO_ZERO                          0x61  
#define ACTION_TYPE_ZOOM_THERMO                 0x62
#define ACTION_TYPE_FIRE_TIMEOUT                0x63
#define ACTION_TO_MODE_STOP						0x71
#define ACTION_TO_MODE_COORD_CONTROL            0x72
#define ACTION_TYPE_POSITION_X_ABS              0x57
#define ACTION_TYPE_POSITION_Y_ABS              0x58

class Action
{
    int action_type;
    long value;

public:
    Action(int new_action_type = ACTION_NOVALID, long new_value = 0)
        : action_type(new_action_type), value(new_value) {}
    void setValue(const long new_value) { value = new_value; }
    void setActionType(const int new_action_type) { action_type = new_action_type; }
    int getActionType() const { return action_type; }
    int getValue() const { return value; }
};

#endif // ACTION_H
