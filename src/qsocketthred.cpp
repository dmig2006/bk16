#include "qsocketthred.h"


QSocketThred::QSocketThred(int descriptor, QObject *parent):QThread(parent),socketDescriptor(descriptor)
{
    socket = new QTcpSocket();
    socket -> setSocketDescriptor(socketDescriptor);
}

QSocketThred::~QSocketThred()
{
    socket -> disconnectFromHost();
    delete socket;
}

void QSocketThred::run()
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),this, SLOT(onDisconnected()),Qt::DirectConnection);
    exec();
}
void QSocketThred::WriteData(char *data, int len)
{

    if(socket->isValid())
    {
        socket->write(data, len);
    }
}
void QSocketThred::onReadyRead()
{
    QByteArray rdata;
    int size = socket -> bytesAvailable();
    rdata = socket->read(size);

    //    Сигнал готовности данных
    emit DataReady(rdata.data(), rdata.size()); //char* data int len
}


void QSocketThred::onDisconnected()
{
    socket -> close();
    quit();
}
