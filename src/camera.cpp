#include "camera.h"
  
/*Конструктор класса*/
Camera::Camera(std::string vid_srv_ip, unsigned short vid_srv_port)
    : srv_ip_addr(vid_srv_ip), srv_port(vid_srv_port),
      is_connect(false), is_operation_mode(true), need_exit(false),
      camera_mode(WIDE_FORMAT)
{
    streams[NARROW_FORMAT] =  "/axis-cgi/mjpg/video.cgi?camera=2";
    streams[WIDE_FORMAT]   =  "/axis-cgi/mjpg/video.cgi?camera=1";
    streams[THERMAL]       =  "/axis-cgi/mjpg/video.cgi?camera=3";
    streams[ZOOM_THERMAL]  =  "/axis-cgi/mjpg/video.cgi?camera=3";
}

/*Деструктор класса*/
Camera::~Camera()
{
    stopExecution();
    wait(1000);
}

/*установка камеры для отображения*/
void Camera::setCamera(CameraMode cam)
{
    mutex.lock();
    camera_mode = cam;
    mutex.unlock();
    reconnect();
}

/*установка флага дежурного/рабочего режима*/
void Camera::setOperationMode(bool op_mode)
{
    mutex.lock();
    is_operation_mode = op_mode;
    if (!is_operation_mode) {
        emit closeCamera();
        closeConnection();
    }
    mutex.unlock();
}

/*получение состояния подключения*/
bool Camera::isConnect()
{
    mutex.lock();
    bool ret = is_connect;
    mutex.unlock();
    return ret;
}

/*установка флага выхода*/
void Camera::stopExecution()
{
    need_exit = true;
}

/*цикл чтения кадров*/
void Camera::run()
{
    while (!need_exit) {
        mutex.lock();
        bool isConnect = is_connect;
        bool isOperationMode = is_operation_mode;
        mutex.unlock();
        if (!isOperationMode) {
            sleep(1);
            continue;
        }
        if (!isConnect) {
            usleep(500000);
            establishConnection();
            continue;
        }
        QImage frame;
        unsigned char ret = readFrame(frame);
        if (ret != NO_ERROR) {
            emit closeCamera();
            closeConnection();
            setError(ret);
        }
        emit newFrame(frame);
    }
    closeStream();
}

/*установка соединения*/
unsigned char Camera::establishConnection()
{
    static bool is_first_connection = true;
    unsigned char ret;
    ret = initStream(srv_ip_addr, srv_port, streams[camera_mode]);
    if (ret == NO_ERROR) {
        is_connect = true;
        if (is_first_connection) {
            is_first_connection = false;
            emit gotConnection();
        }
    } else {
        is_connect = false;
        setError(ret);
        sleep(1);
    }
    return ret;
}

/*закрытие соединения*/
void Camera::closeConnection()
{
    is_connect = false;
    closeStream();
}

/*возобновление соединения*/
bool Camera::reconnect()
{
    static int count = 0;
    unsigned char ret = NO_ERROR;
    mutex.lock();
    closeConnection();
    if (count < 2)
        ++count;
    else
        ret = establishConnection();
    mutex.unlock();
    return (ret == NO_ERROR) ? true : false;
}

/*обработка ошибок*/
void Camera::setError(const unsigned char error_id)
{
    switch (error_id) {
    case ERROR_OPENING_SOCKET:
        //emit newLog("CAMERA ERROR: OPENING SOCKET");
        break;
    case ERROR_BAD_ADDRESS:
        //emit newLog("CAMERA ERROR: BAD ADDRESS");
        break;
    case ERROR_CONNECTING_TO_SERVER:
        //emit newLog("CAMERA ERROR: CONNECTING TO SERVER");
        break;
    case ERROR_QUERY_VIDEO:
        //emit newLog("CAMERA ERROR: QUERY VIDEO");
        break;
    case ERROR_READING_STREAM:
        //emit newLog("CAMERA ERROR: READING STREAM");
        break;
    default:
        break;
    }
}
