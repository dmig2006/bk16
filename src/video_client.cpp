#include "video_client.h"

/*инициализация соединения*/
unsigned char VideoClient::initStream(const std::string ip,
                                      const unsigned short port,
                                      const std::string stream)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        return ERROR_OPENING_SOCKET;

    int ret;
    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    if (inet_pton(AF_INET, ip.c_str(), &srv_addr.sin_addr) <= 0) {
        closeStream();
        return ERROR_BAD_ADDRESS;
    }
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port); 

    ret = connect(sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    if (ret == -1) {
        closeStream();
        return ERROR_CONNECTING_TO_SERVER;
    }
    return sendRequest(stream);
}

/*закрытие соединения*/
void VideoClient::closeStream()
{
    if (sock != -1) {
        close(sock);
        sock = -1;
    }
}

/*чтнеие кадра*/
unsigned char VideoClient::readFrame(QImage &img)
{
    int size = getFrameSize();
    if (size == -1)
        return ERROR_READING_STREAM;

    unsigned char *ptr = new unsigned char [size];
    int total = 0;
    int partial = 0;
    while (total < size) {
        partial = readData(ptr + total, size - total);
        if (partial == -1) {
            delete [] ptr;
            ptr = 0;
            return ERROR_READING_STREAM;
        }
        total += partial;
    }
    img.loadFromData(ptr, size, "jpg");
    delete [] ptr;
    return NO_ERROR;
}

/*отпарвка запроса*/
unsigned char VideoClient::sendRequest(const std::string stream)
{
    std::string request = "GET " + stream + "\r\n\r\n";
    int ret = writeData(reinterpret_cast<const unsigned char*>(request.c_str()), request.size());
    if (ret <= 0) {
        return ERROR_QUERY_VIDEO;
    }
    return NO_ERROR;
}

/*получение размера кадра*/
int VideoClient::getFrameSize()
{
    std::string str;
    int ret_val = -1;
    for (;;) {
        if (readLine(str) == -1) {
            std::cout << str << std::endl;
            break;
        }
        if (!str.length() || str.find("Content-Length") == std::string::npos)
            continue;
        std::stringstream ss;
        ss << str;
        std::string tmp;
        ss >> tmp >> ret_val;
        readLine(str);
        break;
    }
    return ret_val;
}

/*чтение строки*/
int VideoClient::readLine(std::string &str)
{
    unsigned char ch;
    int num_read;
    str.clear();
    for(;;) {
        num_read = readData(&ch, 1);
        if (num_read == -1) {
            str.clear();
            return -1;
        } else if (num_read == 0) {
            if (str.size() == 0)
                return 0;
            else
                break;
        } else {
            str.push_back(ch);
            if (ch == '\n')
                break;
        }
    }
    str.push_back('\0');
    return str.size();
}

/*чтение*/
int VideoClient::readData(unsigned char *data, int size)
{
    struct pollfd fds;
    fds.fd = sock;
    fds.events = POLLIN;
    if (poll(&fds, 1, 500) <= 0 || fds.revents & (POLLERR | POLLHUP | POLLNVAL))
        return -1;
    return recv(sock, data, size, MSG_NOSIGNAL);
}

/*запись*/
int VideoClient::writeData(const unsigned char *data, int size)
{
    struct pollfd fds;
    fds.fd = sock;
    fds.events = POLLOUT;
    if (poll(&fds, 1, 500) <= 0 || fds.revents & (POLLERR | POLLHUP | POLLNVAL))
        return -1;
    return send(sock, data, size, MSG_NOSIGNAL);
}
