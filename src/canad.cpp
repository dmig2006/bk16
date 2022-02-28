#include "qendian.h"
#include "canad.h"

canad::canad(QObject *parent) :
    QObject(parent)
{
    int i;
    //Очистка входного буфера
    for(i=0; i<CANAD_BYTEPACKET_MAXSIZE; i++){ pkbuf_rx[i] = 0;}
    bcnt = 0;
    bcnt_max = CANAD_BYTEPACKET_MAXSIZE;

    MakeCRC32Table();

    //Error counters
    err_bytes_rejected = 0; //Колличество отброшенных байтов пакетов
    err_wrong_crc = 0;      //Колличество пакетов с не совпавшим CRC
}
//Функция формирования таблицы значений для рассчета CRC32.
void canad::MakeCRC32Table(void)
{
    int i, j;
    uint c, cr;
    for(i=0; i < 256; ++i)
    {
        cr = i;
        c = i << 24;
        for(j = 8; j > 0; --j)
        {
            c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
            cr = cr & 0x00000001 ? (cr >> 1) ^ CRC32_POLY_R : (cr >> 1);
        }
        crc32_table[i] = c;
        crc32r_table[i] = cr;
    }
}
//Функция рассчета CRC32 как в аппаратном блоке STM32. Входные данные должны быть вывровненны по u32
uint canad::GetCRC32(uint* data, int len)
{
    uint v = 0;
    uint crc;
    crc = 0xFFFFFFFF;
    while(len >= 4)
    {
        v = qToBigEndian(*data++);
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v ) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 8) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 16) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 24) )];
        len -= 4;
    }
    if(len)
    {
        switch(len)
        {
            case 1: v = 0xFF000000 & qToBigEndian(*data++);
                break;
            case 2: v = 0xFFFF0000 & qToBigEndian(*data++);
                break;
            case 3: v = 0xFFFFFF00 & qToBigEndian(*data++);
                break;
        }
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v ) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 8) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 16) )];
        crc = ( crc << 8 ) ^ crc32_table[0xFF & ( (crc >> 24) ^ (v >> 24) )];
    }
    return ~crc;
}
//http://www.fileformat.info/tool/hash.htm?hex=3132333435363738
//Стандартная функция рассчета CRC32
/*uint canad::GetCRC32_Native(char* data, int len)
{
    uint v;
    uint crc;
    crc = 0xFFFFFFFF;
    while(len > 0) {
        v = *data++;
        crc = ( crc >> 8 ) ^ crc32r_table[( crc ^ (v ) ) & 0xff];
        len --;
    }
    return ~crc;
}*/


//Функция разбора потока байтов, выделение пакетов и формировния CAN-фрейма
//Receive bytestream from port, extract can messages, send to top level
void canad::BytesToFrames(char* data, int size)
{
    int i = 0;
    //Структура принятого пакета
    //[AB][BA][DLC][IDL][IDH][D0][D1][D2][D3][D4][D5][D6][D7][CRCL][CRCH]
    //pkbuf - буфер собираемого пакета
    //pkbuf[0] =    [DLC]
    //pkbuf[1] =    [IDL]
    //pkbuf[2] =    [IDH]
    //pkbuf[3] =    [D0]
    //pkbuf[4] =    [D1]
    //pkbuf[5] =    [D2]
    //pkbuf[6] =    [D3]
    //pkbuf[7] =    [D4]
    //pkbuf[8] =    [D5]
    //pkbuf[9] =    [D6]
    //pkbuf[10] =   [D7]
    //pkbuf[11] =   [CRC16L]
    //pkbuf[12] =   [CRC16H]

    while(i<size)   //make out all bytes in current piece - Разбираем все байты в заданном блоке
    {
        //Catch SOP (Start-of-Packet) Ловим старт пакет
        if(!(bcnt & (~1))){  //if SOP expected (bcnt = 0 or bcnt = 1)
            if(!bcnt)
            {  //if bcnt = 0
                if( data[i] == char(CANAD_STARTBYTE_H) ){ bcnt++; }
                else
                {   //if SOP not here, go to next byte
                    err_bytes_rejected++;
                    emit SendToLog(QString("CANAD: Rejected %1").arg(err_bytes_rejected));
                }
            }
            else
            {   //if bcnt = 1
                if( data[i] == char(CANAD_STARTBYTE_L) )
                {   //if startbytes ok
                    //if SOP is OK here
                    //pkbuf_rx[0] = CANAD_STARTBYTE_H;//AB
                    //pkbuf_rx[1] = CANAD_STARTBYTE_L;//BA
                    bcnt++;
                    bcnt_max = CANAD_BYTEPACKET_MAXSIZE-1;
                    //emit SendToLog(QString("SOP_OK"));
                }
                else
                {   //if second startbyte is wrong
                    bcnt = 0;
                    err_bytes_rejected++;
                    emit SendToLog(QString("CANAD: Rejected %1").arg(err_bytes_rejected));
                }
            }
        }
        else
        {   //if packet body expected - старт байты приняты ловим тело пакета
            pkbuf_rx[bcnt-2] = data[i];

            if(bcnt == 2)
            {   //based on frame data length, determine bcnt_max for current packet
                bcnt_max = data[i] + 7;
                //7 is a number of bytes in packet, except data payload: 2 startbytes, size, 2 bytes of addr, 2 bytes of crc16
            }
            bcnt++;
            if(bcnt >= bcnt_max)
            {   //if EOP is expected - если принят последний байт пакета
                //Check CRC16
                uint16_t crc16in; //принятый CRC
                crc16in = (uint16_t)(pkbuf_rx[bcnt_max-1-2]);
                crc16in<<=8;
                crc16in |= ((uint16_t)(pkbuf_rx[bcnt_max-2-2]))&0xFF;
                //emit SendToLog(QString("CRC in=0x%1").arg(crc16in,0,16));

                uint crc32my;   //рассчетный CRC
                crc32my = GetCRC32((uint*)(&(pkbuf_rx[0])), bcnt_max-4);//four bytes: 2xSB, CRCH,CRCL
                uint16_t crc16my;
                crc16my = crc32my;  //берем тольок младшие 16 бит от CRC32_STM32 (aka crc32my &= 0x0000FFFF)
                //emit SendToLog(QString("CRC my=0x%1").arg(crc16my,0,16));

                if(crc16in == crc16my)
                {   //if CRC match
                    //Формируем CAN-фрейм
                    rxframe.can_dlc = pkbuf_rx[0];
                    uint16_t id;
                    id = pkbuf_rx[2]; id<<=8; id |= ((uint16_t)(pkbuf_rx[1]))&0xFF;
                    rxframe.can_id = id;
                    //Копируем данные
                    for(int u = 0; u < rxframe.can_dlc; u++)
                    {
                        rxframe.data[u] = pkbuf_rx[u+3];
                    }

                    emit SendToLog(QString("SP id=%1 dlc=%2 data=0x%3 %4 %5 %6 %7 %8 %9 %10")
                                   .arg(rxframe.can_id)
                                   .arg(rxframe.can_dlc)
                                   .arg(rxframe.data[0],0,16)
                                   .arg(rxframe.data[1],0,16)
                                   .arg(rxframe.data[2],0,16)
                                   .arg(rxframe.data[3],0,16)
                                   .arg(rxframe.data[4],0,16)
                                   .arg(rxframe.data[5],0,16)
                                   .arg(rxframe.data[6],0,16)
                                   .arg(rxframe.data[7],0,16));
                    //Отправляем CAN-фрейм
                    emit SendFrame(&rxframe);
                    FramesToBytes(&rxframe);
                }
                else
                {   //If CRC not match
                    err_wrong_crc++;
                    emit SendToLog(QString("CANAD: CRC errors %1, expected 0x%2, received 0x%3")
                        .arg(err_wrong_crc).arg(crc16my).arg(crc16in));
                }
                bcnt = 0;//Expect next packet
            }
        }
        i++;
    };
}

//Функция формирования пакета для отправки CAN-фрейма
void canad::FramesToBytes(can_frame* frame)
{
    int i;
    pkbuf_tx[0] = CANAD_STARTBYTE_H;    //AB
    pkbuf_tx[1] = CANAD_STARTBYTE_L;    //BA

    pkbuf_tx[2] = frame->can_dlc;       //DLC
    pkbuf_tx[3] = (frame->can_id);      //IDL
    pkbuf_tx[4] = (frame->can_id)>>8;   //IDH

    for(i=0; i<frame->can_dlc; i++)
    {   //Данные
        pkbuf_tx[2+1+2+i] = frame->data[i];
    }

    //Буфер данных для рассчета CRC32(Выровненные данные)
    char crcbuf[1+2+frame->can_dlc];
    for(i=0; i<(1+2+frame->can_dlc); i++)
    {
        crcbuf[i] = pkbuf_tx[i+2];
    }
    //Рассчитываем CRC32 для DLC, IDL, IDH è DATA
    uint crc32my = GetCRC32((uint*)(crcbuf), 1+2+frame->can_dlc);
    uint16_t crc16my = crc32my;
    //Добавляем рассчитанный CRC в пакет
    pkbuf_tx[2+1+2+frame->can_dlc] = crc16my;   //CRCL
    pkbuf_tx[2+1+2+frame->can_dlc+1] = crc16my>>8;   //CRCH

    emit SendBytes((char*)(pkbuf_tx), frame->can_dlc+2+1+2+2);
}
