#include "tcpclient.h"

/*Конструктор класса*/
TCPClient::TCPClient(std::string srv_ip_addr, unsigned short srv_port)
{
    open(srv_ip_addr, srv_port);
}

/*Деструктор класса*/
TCPClient::~TCPClient()
{
    close();
}

/*открытие соединения*/
void TCPClient::open(std::string srv_ip_addr, unsigned short srv_port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw TCPConnectionError();
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    if (inet_pton(AF_INET, srv_ip_addr.c_str(), &srv_addr.sin_addr) <= 0) {
        ::close(sock);
        throw TCPConnectionError();
    }
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(srv_port);

    if (::connect(sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        ::close(sock);
        throw TCPConnectionError();
    }
}

/*закрытие соединения*/
void TCPClient::close()
{
    ::close(sock);
}

/*чтение команды*/
Action* TCPClient::readAction()
{
    int ret;
    Action *action = new Action();
    ret = recv(sock, action, sizeof(Action), MSG_NOSIGNAL);
    if (ret <= 0) {
        delete action;
        throw TCPIOError();
    }
    return action;
}

/*запись команды*/
void TCPClient::writeAction(Action *action)
{
    int ret;
    ret = send(sock, action, sizeof(Action), MSG_NOSIGNAL);
    if (ret == -1) {
        delete action;
        throw TCPIOError();
    }
    delete action;
}
