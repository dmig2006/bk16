#ifndef QSOCKETTHRED_H
#define QSOCKETTHRED_H

#include <QTcpSocket>
#include <QThread>
#include <QTime>

class QSocketThred : public QThread
{
    Q_OBJECT
public:
    explicit QSocketThred(int descriptor, QObject *parent = 0);
    ~QSocketThred();

    void run();
signals:
    void DataReady(char* data, int len);
    void newLogMsg(QString str);


public slots:
    void WriteData(char* data, int len);    //отправка данных в сеть
    void onReadyRead();
    void onDisconnected();
private:
    int socketDescriptor;
    QTcpSocket* socket;

    bool need_exit;

};

#endif // QSOCKETTHRED_H
