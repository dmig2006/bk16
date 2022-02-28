#ifndef CAMERA_H
#define CAMERA_H

#include <QThread>
#include <QMutex>
#include <QImage>

#include <iostream>
#include <string>
#include <map>

#include "camera_error.h"
#include "video_client.h"
#include "view.enum.h"

class Camera : public QThread, protected VideoClient
{
    Q_OBJECT

    QMutex mutex;

    std::map<CameraMode, std::string> streams;
    std::string srv_ip_addr;
    unsigned short srv_port;
    bool is_connect;
    bool is_operation_mode;
    bool need_exit;
    CameraMode camera_mode;

    unsigned char establishConnection();
    void closeConnection();
    bool reconnect();
    void setError(const unsigned char error_id);

protected:
    void run();

public:
    Camera(std::string vid_srv_ip = "192.168.0.4",
           unsigned short vid_srv_port = 80);
    ~Camera();
    void setCamera(CameraMode cam);
    void setOperationMode(bool op_mode);
    bool isConnect();

public slots:
    void stopExecution();
    
signals:
    void newFrame(QImage image);
    void closeCamera();
    void newLogMsg(QString string);
    void gotConnection();
};

#endif // CAMERA_H
