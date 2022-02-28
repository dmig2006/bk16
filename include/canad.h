#ifndef CANAD_H
#define CANAD_H

#include <QObject>
#include <stdint.h>

#define CANAD_STARTBYTE_H 0xAB
#define CANAD_STARTBYTE_L 0xBA
#define CANAD_BYTEPACKET_MAXSIZE 15

#define CRC32_POLY      0x04C11DB7
#define CRC32_POLY_R    0xEDB88320

#define CONVERTER_PING_CMD          4500
#define CONVERTER_CMD_BAUD_SET      4501
#define CONVERTER_CMD_CAN_RESET     4502
#define CONVERTER_CMD_SYS_RESET     4503
#define CONVERTER_CMD_UART_BAUD_SET 4504

struct can_frame{
    unsigned int can_id;    //11 bit CAN_ID
    unsigned char can_dlc;  //data length code: 0..8
    unsigned char data[8] __attribute__((aligned(8)));
};
//Canad Packet structure:
//Startbytes    0xAB 0xBA
//CAN_DLC       1 byte
//CAN_ID        1 byte
//DATA          0..8 bytes
//CRC           2 bytes

class canad : public QObject
{
    Q_OBJECT
public:
    explicit canad(QObject *parent = 0);
    void MakeCRC32Table(void);
    uint GetCRC32(uint *data, int len);

    can_frame rxframe;
    uint8_t pkbuf_rx[CANAD_BYTEPACKET_MAXSIZE];
    int err_bytes_rejected;
    int err_wrong_crc;
    
    uint8_t pkbuf_tx[CANAD_BYTEPACKET_MAXSIZE];    
    
private:
    int bcnt;
    int bcnt_max;

    uint crc32_table[256];
    uint crc32r_table[256];
signals:
    void SendToLog(QString);
    void SendFrame(can_frame*); //ОТправка принятого фрейма другим классам
    void SendBytes(char* data, int len); //Отправка байтов в порт
public slots:
    void BytesToFrames(char* data, int size);   //Прием потока байтов в порт
    void FramesToBytes(can_frame* frame);   //Прием фрейма от других классов
};

#endif // CANAD_H
