 #include "qsimpleserver.h"

QSimpleServer::QSimpleServer(QThread *parent):QThread(parent)

{
    udpsocket = new QUdpSocket;
    if (udpsocket->bind(QHostAddress::AnyIPv4,48108,QAbstractSocket::ShareAddress)) //связывание порта с адресом при получении посылки отправляется сигнал readyread();
    {
        //qDebug() << "сокет открыт";
    }
    //else qDebug() << "сокет закрыт";
    can_adapter = new canad;

    connect(can_adapter, SIGNAL(SendBytes(char*,int)), this, SLOT(WriteData(char*,int)));
    connect(this, SIGNAL(DataReady(char*,int)), can_adapter, SLOT(BytesToFrames(char*,int)));
    connect(can_adapter, SIGNAL(SendFrame(can_frame*)), this, SLOT(OnFrameReady(can_frame*)));
}

void QSimpleServer::run()
{
    connect (udpsocket, SIGNAL(readyRead()),this,SLOT(readyReadUdp()),Qt::DirectConnection);
    exec();
}

void QSimpleServer::readyReadUdp()
{
    //qDebug() << "посылку получил";
    while (udpsocket->hasPendingDatagrams())
    {
        QByteArray   buffer;
        QHostAddress sender;
        quint16      senderPort;
        QString      str;

        buffer.resize(udpsocket->pendingDatagramSize());
        udpsocket->readDatagram(buffer.data(),buffer.size(),&sender, &senderPort);


        str.append(sender.toString()).append(" : ").append(QString::number(senderPort)).append(" : ").append(QString::number(buffer.size())).append(" ... ");//.append(buffer);
        //qDebug() << str;
        emit DataReady(buffer.data(), buffer.size()); //char* data int len
    }
}

QSimpleServer::~QSimpleServer()
{
    udpsocket->disconnectFromHost();
    delete udpsocket;
    delete can_adapter;
}

void QSimpleServer::FramesToBytes(can_frame* frame)
{
//    добавить проверку наличия потока
    can_adapter->FramesToBytes(frame);
}

void QSimpleServer::WriteData(char * data, int len)
{
    if(udpsocket->isValid())
    {
        int i=0;
        i=i;
        i = udpsocket->writeDatagram(data,(quint64)len,QHostAddress("192.168.0.111"),48107);
    }
}
void QSimpleServer::OnFrameReady(can_frame *frame)
{
    emit FrameReady(frame);
}
