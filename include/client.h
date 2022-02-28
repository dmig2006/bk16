#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>

#include "camera_error.h"

class Client
{
    int  sock;

public:
    Client() : sock(-1) {}
    ~Client() { close(); }
    int connectToServer(const std::string ip,
    	                const unsigned short port);
    void close();
    bool writeBytes(const void *data, size_t len);
    bool readBytes(void *data, size_t len);
};

#endif // Client_H
