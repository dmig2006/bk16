#ifndef PID_H
#define PID_H

#include <QDialog>

#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QString>

#include "controllermanager.h"

namespace Ui {
class PID;
}

struct PID_DATA
{
    float Usta;		//уставка
    float Zona;		//зона не чувств.
    float fTmls;	//время фильтра остановки мсек.
    int	  Tmls;		//время дискретизации мсек.

    float Dves;		//Вес дифференциальной
    float Pves;		//Вес пропорциональной (1 по умолчанию - "нет веса"

    float Pkff_z0;		//Пропорциональная
    float Ikff_z0;		//Время интегрирования в мсек???
    float Dkff_z0;		//Дифференциальная
    float Pkff_z1;		//Пропорциональная
    float Ikff_z1;		//Время интегрирования в мсек???
    float Dkff_z1;		//Дифференциальная
    float Pkff_z2;		//Пропорциональная
    float Ikff_z2;		//Время интегрирования в мсек???
    float Dkff_z2;		//Дифференциальная

    int   MaxU;		//Максимльное упр. воздействие
    int   MinU;		//Минимальное упр. воздействие
    bool  OnOf;		//1 - ВКЛ., 0 - ВЫКЛ.
    char  Zona_kff;     //зоны смены коэффициентов ПИД-регулятора
    int   Shag;		//шаг записи в массив расхождения

    float Rash[3];
    float Rash_S[3];

    float PidV;		//ПИД выходное значение
    float PidV_stop;//Значение остановки ПИД-регулятора, фильтра

    float P,I,D;	//отладочные переменные
    float R1,R2,R3; //отладочные переменные
};


class PID : public QDialog
{
    Q_OBJECT
    //PID_DATA pid;

public:
    explicit PID(QWidget *parent = 0);
    ~PID();
    void test(void);
    bool flag_ac;
    PID_DATA pid;
    Ui::PID *ui;
    bool page_pid;
    float fT;

    void colorInversia(bool pFlag);

    void znachenieStart(float b1X,  float b1Y,  float b2X,  float b2Y,  float b3X,  float b3Y,  float b4X,  float b4Y,  float b5X,  float b5Y,
                        float b6X,  float b6Y,  float b7X,  float b7Y,  float b8X,  float b8Y,  float b9X,  float b9Y,  float b10X, float b10Y,
                        float b11X, float b11Y, float b12X, float b12Y, float b13X, float b13Y, float b14X, float b14Y, float b15X, float b15Y,
                        float b16X, float b16Y);

private:

    QTimer *timer;

signals:
    void pid_value  (float value);
    void readVarPid(float pv,float p,float i,float d/*,float r1,float r2,float r3*/);
    void readValuePid(float pv);

    void readPidX(float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                  float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);
    void readPidY(float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                  float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);

    void newLog(QString str);

public slots:
    void pid_on_off (bool enabled, float Fact_Zn);

    void read_pid_X(float pv,float p,float i,float d/*,float r1,float r2,float r3*/);
    void read_pid_Y(float pv,float p,float i,float d/*,float r1,float r2,float r3*/);

    void writePidX (float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                    float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);
    void writePidY (float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                    float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);

    void writePageX(float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                    float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);
    void writePageY(float b1, float b2,  float b3,  float b4,  float b5,  float b6,  float b7,  float b8,
                    float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16);

    void on_Exit_clicked();
    void on_Apply_clicked();

    void closePid();
};

#endif // PID_H
