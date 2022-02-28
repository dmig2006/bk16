#ifndef VIDEO_CLIENT_H
#define VIDEO_CLIENT_H

#include <QImage>

#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#include "camera_error.h"

class VideoClient
{
    int sock;
    
    unsigned char sendRequest(const std::string stream);
    int getFrameSize();
    int readLine(std::string &str);
    int readData(unsigned char *data, int size);
    int writeData(const unsigned char *data, int size);

public:
    VideoClient() : sock(-1) {}
    ~VideoClient() { closeStream(); }
    unsigned char initStream(const std::string ip,
                             const unsigned short port,
                             const std::string stream);
    void closeStream();
    unsigned char readFrame(QImage &img);
};

#endif // VIDEO_CLIENT_H
