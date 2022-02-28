#ifndef WEAPON_ENUM_H
#define WEAPON_ENUM_H

#define BURST_TYPE_COUNT    2   // BurstType count

enum WeaponType {
    WEAPON_KORD,
    WEAPON_PKT,
    WEAPON_AGS,
    WEAPON_AG40
};

enum BurstType {
    SHORT_BURST,
    LONG_BURST
};

enum FireType {
    SINGLE_FIRE,
    TURN_FIRE
};

enum Mode {
    MODE_AUTO,
    MODE_SEMI_AUTO,
    MODE_TRACKING,
    MODE_MARCH
};

enum Cocking {
    COCKING_LOAD,
    COCKING_COMPLETED, // completed
    COCKING_ERROR      // error
};

enum AutoType {
    AUTO_TYPE_ORDER,      // In saving order
    AUTO_TYPE_COORDINATE  // By horizontal coordinate
};

#endif // WEAPON_ENUM_H
