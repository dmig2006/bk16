#ifndef AIM_H
#define AIM_H

struct Aim {
    long mPositionX;
    long mPositionY;
    long mRange;
    float mCorrectionX;
    float mCorrectionY;
    bool mState;
    bool mActive;
    bool isAlive;
    int mTarget;
};

#endif // AIM_H
