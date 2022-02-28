#ifndef COM_PORT
#define COM_PORT
/*****************************************************************************/
/*** COM port for key control                                              ***/
/*****************************************************************************/
#include <QMutex>

#include <iostream>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "crc8.h"

class ComPort
{
    int mFd;
    QMutex mMutex;

public:
    ComPort() : mFd(0) {}
    bool open(char* ptty);
    bool close();
    bool read(unsigned char *pBytes, int pLen);
    bool write(unsigned char *pBytes, int pLen);
};

#endif // COM_PORT
