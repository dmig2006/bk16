#include "comport.h"

/*открытие последовательного порта*/
bool ComPort::open(char* ptty)
{
    mFd = ::open(ptty, O_RDWR | O_NOCTTY );
    if (mFd == -1) {
        return false;
    } else {
        fcntl(mFd, F_SETFL, 0);
    }

    struct termios port_settings;
    tcgetattr(mFd, &port_settings);
    cfsetispeed(&port_settings, B115200);
    cfsetospeed(&port_settings, B115200);
    port_settings.c_cflag |= (CLOCAL | CREAD);
    port_settings.c_cflag &= ~PARENB;
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
    port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    cfmakeraw(&port_settings);
    port_settings.c_cc[VTIME] = 10;
    port_settings.c_cc[VMIN] = 0;
    tcsetattr(mFd, TCSANOW, &port_settings);
    tcflush(mFd, TCIOFLUSH);

    return true;
}

/*закрытие последовательного порта*/
bool ComPort::close()
{
    int n;
    n = ::close(mFd);
    return (n == -1) ? false : true;
}

/*чтение*/
bool ComPort::read(unsigned char *pBytes, int pLen)
{
    int n;
    mMutex.lock();
    n = ::read(mFd, pBytes, pLen);
    mMutex.unlock();
    return (n == -1) ? false : true;
}

/*запись*/
bool ComPort::write(unsigned char *pBytes, int pLen)
{
    int n;
    mMutex.lock();
    n = ::write(mFd, pBytes, pLen);
    mMutex.unlock();
    return (n == -1) ? false : true;
}
