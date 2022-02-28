#include "client.h"

/*открытие соединения*/
int Client::connectToServer(const std::string ip,
                            const unsigned short port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return ERROR_OPENING_SOCKET;

    int ret;
    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    if (inet_pton(AF_INET, ip.c_str(), &srv_addr.sin_addr) <= 0) {
        close();
        return ERROR_BAD_ADDRESS;
    }
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port); 

    ret = connect(sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (ret == -1) {
        close();
        return ERROR_CONNECTING_TO_SERVER;
    }
    return NO_ERROR;
}

/*закрытие соединения*/
void Client::close()
{
    if (sock != -1) {
        ::close(sock);
        sock = -1;
    }
}

/*запись*/
bool Client::writeBytes(const void *data, size_t len)
{
    struct pollfd fds;
    fds.fd = sock;
    fds.events = POLLOUT;
    if (poll(&fds, 1, 5000) <= 0 || fds.revents & (POLLERR | POLLHUP | POLLNVAL))
        return false;
    int ret = send(sock, data, len, MSG_NOSIGNAL);
    return !(ret < 0);
}

/*чтение*/
bool Client::readBytes(void *data, size_t len)
{
    struct pollfd fds;
    fds.fd = sock;
    fds.events = POLLIN;
    if (poll(&fds, 1, 5000) <= 0 || fds.revents & (POLLERR | POLLHUP | POLLNVAL))
        return false;
    int ret = recv(sock, data, len, MSG_NOSIGNAL);
    return !(ret <= 0);
}
