#ifndef TCPCLIENT_H
#define TCPCLIENT_H
/*****************************************************************************/
/*** TCP/IP client                                                         ***/
/*****************************************************************************/
#include <string>
#include <cstring>

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "crc8.h"
#include "action.h"
#include "actionlist.h"

class TCPError {};
class TCPConnectionError : public TCPError {};
class TCPIOError : public TCPError {};

class TCPClient
{
    static TCPClient *tcp_client;
    int sock;

    void open(std::string srv_ip_addr, unsigned short srv_port);
    void close();

public:
    TCPClient(std::string srv_ip_addr, unsigned short srv_port);
    ~TCPClient();
    Action* readAction();
    void writeAction(Action *action);
};

#endif // TCPCLIENT_H
