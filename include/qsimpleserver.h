#ifndef TCPSERV_H
#define TCPSERV_H

#include <QThread>
#include <QUdpSocket>
#include  "canad.h"

class QSimpleServer : public QThread
{
    Q_OBJECT
public:
    explicit QSimpleServer(QThread *parent =0);

    canad	 *can_adapter;
    void run();

    ~QSimpleServer(); // освобождение памяти canad
private slots:
    void OnFrameReady(can_frame* frame);
    void readyReadUdp();
signals:
    void FrameReady(can_frame* frame);  //сигнал готовности принятого фрейма
    void newLogMsg(QString str);
    void DataReady(char* data, int len);
public slots:
    void FramesToBytes(can_frame* frame);    //слот отправки фрейма
    void WriteData(char* data,int len);

private:
    QUdpSocket *udpsocket;
   
};

#endif // TCPSERV_H
