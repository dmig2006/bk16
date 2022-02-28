#include "pid.h"
#include "ui_pid.h"

PID::PID(QWidget *parent) :
    QDialog(parent), ui(new Ui::PID)
{
    ui->setupUi(this);

    pid.Usta = 0.0;
    pid.PidV = 0.0f;
    pid.OnOf = false;
    pid.Shag = 0;

    pid.Tmls = 40.0f;

    pid.Zona_kff  = 0;
    pid.fTmls     = 40.0f;
    pid.PidV_stop = 50.0f;
    pid.Pves      = 1.0f;
    pid.Dves      = 1.0f;

    pid.P =  0.0f;
    pid.I =  0.0f;
    pid.D =  0.0f;
    pid.R1 = 0.0f;
    pid.R2 = 0.0f;
    pid.R3 = 0.0f;
    flag_ac = false;
    fT = 0.0f;
}

PID::~PID()
{
    delete ui;
}

void PID::znachenieStart(float b1X, float b1Y, float b2X, float b2Y, float b3X, float b3Y, float b4X, float b4Y, float b5X, float b5Y,
                         float b6X, float b6Y, float b7X, float b7Y, float b8X, float b8Y, float b9X, float b9Y, float b10X, float b10Y,
                         float b11X, float b11Y, float b12X, float b12Y, float b13X, float b13Y, float b14X, float b14Y, float b15X,
                         float b15Y, float b16X, float b16Y)
{
    ui -> zonaX   -> setText(QString::number(b1X));
    ui -> tmlsX   -> setText(QString::number(b2X));
    ui -> pvesX   -> setText(QString::number(b3X));
    ui -> dvesX   -> setText(QString::number(b4X));
    ui -> maxuX   -> setText(QString::number(b5X));
    ui -> minuX   -> setText(QString::number(b6X));
    ui -> pkffX_1   -> setText(QString::number(b7X));
    ui -> tkffX_1   -> setText(QString::number(b8X));
    ui -> dkffX_1   -> setText(QString::number(b9X));
    ui -> fTX     ->setText(QString::number(b10X));
    ui -> pkffX_2 -> setText(QString::number(b11X));
    ui -> tkffX_2 -> setText(QString::number(b12X));
    ui -> dkffX_2 -> setText(QString::number(b13X));
    ui -> pkffX_3 -> setText(QString::number(b14X));
    ui -> tkffX_3 -> setText(QString::number(b15X));
    ui -> dkffX_3 -> setText(QString::number(b16X));

    ui -> zonaY   -> setText(QString::number(b1Y));
    ui -> tmlsY   -> setText(QString::number(b2Y));
    ui -> pvesY   -> setText(QString::number(b3Y));
    ui -> dvesY   -> setText(QString::number(b4Y));
    ui -> maxuY   -> setText(QString::number(b5Y));
    ui -> minuY   -> setText(QString::number(b6Y));
    ui -> pkffY_1   -> setText(QString::number(b7Y));
    ui -> tkffY_1   -> setText(QString::number(b8Y));
    ui -> dkffY_1   -> setText(QString::number(b9Y));
    ui -> fTY     -> setText(QString::number(b10Y));
    ui -> pkffY_2 -> setText(QString::number(b11Y));
    ui -> tkffY_2 -> setText(QString::number(b12Y));
    ui -> dkffY_2 -> setText(QString::number(b13Y));
    ui -> pkffY_3 -> setText(QString::number(b14Y));
    ui -> tkffY_3 -> setText(QString::number(b15Y));
    ui -> dkffY_3 -> setText(QString::number(b16Y));
}

void PID::on_Apply_clicked()
{
    QString st;
    float zona, tmls, pves, dves, maxu, minu, pkff0, tkff0, dkff0, fT, pkff1, tkff1, dkff1, pkff2, tkff2, dkff2;
    page_pid = true;

    st = ui -> zonaX -> text();    zona = st.toFloat();
    st = ui -> tmlsX -> text();    tmls = st.toFloat();
    st = ui -> pvesX -> text();    pves = st.toFloat();
    st = ui -> dvesX -> text();    dves = st.toFloat();
    st = ui -> maxuX -> text();    maxu = st.toFloat();
    st = ui -> minuX -> text();    minu = st.toFloat();
    st = ui -> pkffX_1 -> text();    pkff0 = st.toFloat();
    st = ui -> tkffX_1 -> text();    tkff0 = st.toFloat();
    st = ui -> dkffX_1 -> text();    dkff0 = st.toFloat();
    st = ui -> fTX   -> text();    fT    = st.toFloat();
    st = ui -> pkffX_2 -> text();  pkff1 = st.toFloat();
    st = ui -> tkffX_2 -> text();  tkff1 = st.toFloat();
    st = ui -> dkffX_2 -> text();  dkff1 = st.toFloat();
    st = ui -> pkffX_3 -> text();  pkff2 = st.toFloat();
    st = ui -> tkffX_3 -> text();  tkff2 = st.toFloat();
    st = ui -> dkffX_3 -> text();  dkff2 = st.toFloat();
    emit readPidX(zona, tmls, pves, dves, maxu, minu, pkff0, tkff0, dkff0, fT, pkff1, tkff1, dkff1, pkff2, tkff2, dkff2);

    st = ui -> zonaY -> text();    zona = st.toFloat();
    st = ui -> tmlsY -> text();    tmls = st.toFloat();
    st = ui -> pvesY -> text();    pves = st.toFloat();
    st = ui -> dvesY -> text();    dves = st.toFloat();
    st = ui -> maxuY -> text();    maxu = st.toFloat();
    st = ui -> minuY -> text();    minu = st.toFloat();
    st = ui -> pkffY_1 -> text();    pkff0 = st.toFloat();
    st = ui -> tkffY_1 -> text();    tkff0 = st.toFloat();
    st = ui -> dkffY_1 -> text();    dkff0 = st.toFloat();
    st = ui -> fTY   -> text();    fT   = st.toFloat();
    st = ui -> pkffY_2 -> text();  pkff1 = st.toFloat();
    st = ui -> tkffY_2 -> text();  tkff1 = st.toFloat();
    st = ui -> dkffY_2 -> text();  dkff1 = st.toFloat();
    st = ui -> pkffY_3 -> text();  pkff2 = st.toFloat();
    st = ui -> tkffY_3 -> text();  tkff2 = st.toFloat();
    st = ui -> dkffY_3 -> text();  dkff2 = st.toFloat();
    emit readPidY(zona, tmls, pves, dves, maxu, minu, pkff0, tkff0, dkff0, fT, pkff1, tkff1, dkff1, pkff2, tkff2, dkff2);

}

void PID::pid_on_off(bool enabled, float Fact_Zn)
{
    float Rdiap_pl = 0;
    float Rdiap_mi = 0;
    float P_kff 	  ;
    float I_kff		  ;
    float D_kff		  ;

    if (1)
    {
        if (!enabled)		//ПИД\ШИМ выкл.
        {
            pid.PidV   = 0.0f;
            P_kff	   = 0.0f;
            I_kff	   = 0.0f;
            D_kff	   = 0.0f;
            for (int i=0; i<3; i++) {pid.Rash[i] = 0; pid.Rash_S[i] = 0;}
        }

        if (enabled)		//ПИДШИМ вкл.
        {
            Rdiap_pl = pid.Usta + pid.Zona; //рабочий диапазон +
            Rdiap_mi = pid.Usta - pid.Zona; //рабочий диапазон -

            //вычисление не отсортированного массива расхождения
            pid.Rash[pid.Shag] = pid.Usta + Fact_Zn;
            //сортировка массива расхождения
            if (pid.Shag == 0)
            {
                pid.Rash_S[2] = pid.Rash[0];
                pid.Rash_S[1] = pid.Rash[2];
                pid.Rash_S[0] = pid.Rash[1];
            }
            if (pid.Shag == 1)
            {
                pid.Rash_S[2] = pid.Rash[1];
                pid.Rash_S[1] = pid.Rash[0];
                pid.Rash_S[0] = pid.Rash[2];
            }
            if (pid.Shag == 2)
            {
                pid.Rash_S[2] = pid.Rash[2];
                pid.Rash_S[1] = pid.Rash[1];
                pid.Rash_S[0] = pid.Rash[0];
            }
            //циркуляция шагов ....0-1-2-0-1-2.....0-1-2.....
            pid.Shag++;
            if (pid.Shag > 2) pid.Shag = 0;
            //Расчёт коэф П, И, Д
            if ( (Fact_Zn >= Rdiap_pl) or (Fact_Zn <= Rdiap_mi) ) //если факт зн выходит за границы зоны не чувств вычислять КОЭФ
            {
                if (pid.Zona_kff == 0)
                {
                    P_kff = pid.Pkff_z0 * (pid.Rash_S[2] * pid.Pves - pid.Rash_S[1]);
                    I_kff = pid.Rash_S[2] * pid.Tmls / pid.Ikff_z0;
                    D_kff = pid.Dkff_z0 / pid.Tmls * (pid.Dves * pid.Rash_S[2] - pid.Rash_S[1] * 2 + pid.Rash_S[0]);
                }
                if (pid.Zona_kff == 1)
                {
                    P_kff = pid.Pkff_z1 * (pid.Rash_S[2] * pid.Pves - pid.Rash_S[1]);
                    I_kff = pid.Rash_S[2] * pid.Tmls / pid.Ikff_z1;
                    D_kff = pid.Dkff_z1 / pid.Tmls * (pid.Dves * pid.Rash_S[2] - pid.Rash_S[1] * 2 + pid.Rash_S[0]);
                }
                if (pid.Zona_kff == 2)
                {
                    P_kff = pid.Pkff_z2 * (pid.Rash_S[2] * pid.Pves - pid.Rash_S[1]);
                    I_kff = pid.Rash_S[2] * pid.Tmls / pid.Ikff_z2;
                    D_kff = pid.Dkff_z2 / pid.Tmls * (pid.Dves * pid.Rash_S[2] - pid.Rash_S[1] * 2 + pid.Rash_S[0]);
                }

                //отладочн переменные

                pid.PidV = pid.PidV   + P_kff + I_kff + D_kff;
                if (pid.PidV >= pid.MaxU) pid.PidV = pid.MaxU;
                if (pid.PidV <= pid.MinU) pid.PidV = pid.MinU;

                emit readValuePid(pid.PidV);
                fT = 0.0f;
            }

            if ( (Fact_Zn < Rdiap_pl) and (Fact_Zn > Rdiap_mi)) //если факт. зн. в зоне не чувств. -> останов ПП
            {
                fT =  fT + pid.Tmls;
                if  ((fT >= pid.fTmls) and (pid.PidV <= pid.PidV_stop) and (pid.PidV >= -pid.PidV_stop) and (pid.Zona_kff == 0))
                {
                    fT = pid.fTmls;
                    pid.PidV = 0.0f;
                    P_kff    = 0.0f;
                    I_kff    = 0.0f;
                    D_kff    = 0.0f;
                    emit readValuePid(pid.PidV);
                }


            }


        }

    }

}

void PID::read_pid_X(float pv, float p, float i, float d/*, float r1, float r2, float r3*/)
{
    ui->pVX->setText(QString::number(pv));
    ui->pX->setText(QString::number(p));
    ui->iX->setText(QString::number(i));
    ui->dX->setText(QString::number(d));
}

void PID::read_pid_Y(float pv, float p, float i, float d/*, float r1, float r2, float r3*/)
{
    ui->pVY->setText(QString::number(pv));
    ui->pY->setText(QString::number(p));
    ui->iY->setText(QString::number(i));
    ui->dY->setText(QString::number(d));
}

void PID::writePidX(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8, float b9,
                    float b10, float b11, float b12, float b13, float b14, float b15, float b16)
{
    pid.Zona = b1;
    pid.Tmls = b2;
    pid.Pves = b3;
    pid.Dves = b4;
    pid.MaxU = b5;
    pid.MinU = b6;
    pid.Pkff_z0 = b7;
    pid.Ikff_z0 = b8;
    pid.Dkff_z0 = b9;
    pid.fTmls = b10;
    pid.Pkff_z1 = b11;
    pid.Ikff_z1 = b12;
    pid.Dkff_z1 = b13;
    pid.Pkff_z2 = b14;
    pid.Ikff_z2 = b15;
    pid.Dkff_z2 = b16;
}

void PID::writePidY(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8,
                    float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16)
{
    pid.Zona = b1;
    pid.Tmls = b2;
    pid.Pves = b3;
    pid.Dves = b4;
    pid.MaxU = b5;
    pid.MinU = b6;
    pid.Pkff_z0 = b7;
    pid.Ikff_z0 = b8;
    pid.Dkff_z0 = b9;
    pid.fTmls = b10;
    pid.Pkff_z1 = b11;
    pid.Ikff_z1 = b12;
    pid.Dkff_z1 = b13;
    pid.Pkff_z2 = b14;
    pid.Ikff_z2 = b15;
    pid.Dkff_z2 = b16;
}

void PID::writePageX(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8,
                     float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16)
{
    ui -> zonaX -> setText(QString::number(b1));
    ui -> tmlsX -> setText(QString::number(b2));
    ui -> pvesX -> setText(QString::number(b3));
    ui -> dvesX -> setText(QString::number(b4));
    ui -> maxuX -> setText(QString::number(b5));
    ui -> minuX -> setText(QString::number(b6));
    ui -> pkffX_1 -> setText(QString::number(b7));
    ui -> tkffX_1 -> setText(QString::number(b8));
    ui -> dkffX_1 -> setText(QString::number(b9));
    ui -> fTX->setText(QString::number(b10));
    ui -> pkffX_2 -> setText(QString::number(b11));
    ui -> tkffX_2 -> setText(QString::number(b12));
    ui -> dkffX_2 -> setText(QString::number(b13));
    ui -> pkffX_3 -> setText(QString::number(b14));
    ui -> tkffX_3 -> setText(QString::number(b15));
    ui -> dkffX_3 -> setText(QString::number(b16));
}

void PID::writePageY(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8,
                     float b9, float b10, float b11, float b12, float b13, float b14, float b15, float b16)
{
    ui -> zonaY -> setText(QString::number(b1));
    ui -> tmlsY -> setText(QString::number(b2));
    ui -> pvesY -> setText(QString::number(b3));
    ui -> dvesY -> setText(QString::number(b4));
    ui -> maxuY -> setText(QString::number(b5));
    ui -> minuY -> setText(QString::number(b6));
    ui -> pkffY_1 -> setText(QString::number(b7));
    ui -> tkffY_1 -> setText(QString::number(b8));
    ui -> dkffY_1 -> setText(QString::number(b9));
    ui -> fTY->setText(QString::number(b10));
    ui -> pkffY_2 -> setText(QString::number(b11));
    ui -> tkffY_2 -> setText(QString::number(b12));
    ui -> dkffY_2 -> setText(QString::number(b13));
    ui -> pkffY_3 -> setText(QString::number(b14));
    ui -> tkffY_3 -> setText(QString::number(b15));
    ui -> dkffY_3 -> setText(QString::number(b16));
}

void PID::on_Exit_clicked()
{
    close();
    page_pid = false;
}

void PID::closePid()
{
    close();
}

void PID::colorInversia(bool pFlag)
{
    if (pFlag)
    {
        ui -> label_TittleX -> setStyleSheet(COLOR_LABEL_NORMA);

        ui -> label_PvX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_IX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pVX -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> pX -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> iX -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> dX -> setStyleSheet(COLOR_LABEL_NORMA_DATA);

        ui -> label_TmlsX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_FTX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_MaxUX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> tmlsX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> fTX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> maxuX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_PvesX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DvesX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_MinUX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pvesX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dvesX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> minuX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_PkffX_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PkffX_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PkffX_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pkffX_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> pkffX_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> pkffX_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_TkffX_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_TkffX_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_TkffX_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> tkffX_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> tkffX_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> tkffX_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_DkffX_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DkffX_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DkffX_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> dkffX_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dkffX_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dkffX_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_ZonaX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> zonaX -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);


        ui -> label_TittleY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PvY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_IY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pVY -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> pY -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> iY -> setStyleSheet(COLOR_LABEL_NORMA_DATA);
        ui -> dY -> setStyleSheet(COLOR_LABEL_NORMA_DATA);

        ui -> label_TmlsY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_FTY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_MaxUY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> tmlsY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> fTY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> maxuY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_PvesY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DvesY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_MinUY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pvesY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dvesY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> minuY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_PkffY_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PkffY_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_PkffY_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pkffY_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> pkffY_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> pkffY_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_TkffY_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_TkffY_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_TkffY_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> tkffY_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> tkffY_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> tkffY_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_DkffY_1 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DkffY_2 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> label_DkffY_3 -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> dkffY_1 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dkffY_2 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);
        ui -> dkffY_3 -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> label_ZonaY -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> zonaY -> setStyleSheet(COLOR_EDIT_NORMA_ACTIVATE);

        ui -> Apply -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> Exit -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
    }
    else
    {
        ui -> label_TittleX -> setStyleSheet(COLOR_LABEL_INVERSIA);

        ui -> label_PvX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_IX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DX -> setStyleSheet(COLOR_LABEL_NORMA);
        ui -> pVX -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> pX -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> iX -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> dX -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);

        ui -> label_TmlsX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_FTX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_MaxUX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> tmlsX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> fTX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> maxuX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_PvesX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DvesX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_MinUX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> pvesX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dvesX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> minuX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_PkffX_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PkffX_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PkffX_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> pkffX_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> pkffX_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> pkffX_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_TkffX_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_TkffX_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_TkffX_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> tkffX_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> tkffX_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> tkffX_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_DkffX_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DkffX_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DkffX_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> dkffX_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dkffX_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dkffX_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_ZonaX -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> zonaX -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);


        ui -> label_TittleY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PvY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_IY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> pVY -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> pY -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> iY -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);
        ui -> dY -> setStyleSheet(COLOR_LABEL_INVERSIA_DATA);

        ui -> label_TmlsY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_FTY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_MaxUY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> tmlsY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> fTY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> maxuY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_PvesY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DvesY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_MinUY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> pvesY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dvesY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> minuY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_PkffY_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PkffY_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_PkffY_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> pkffY_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> pkffY_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> pkffY_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_TkffY_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_TkffY_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_TkffY_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> tkffY_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> tkffY_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> tkffY_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_DkffY_1 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DkffY_2 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> label_DkffY_3 -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> dkffY_1 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dkffY_2 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);
        ui -> dkffY_3 -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> label_ZonaY -> setStyleSheet(COLOR_LABEL_INVERSIA);
        ui -> zonaY -> setStyleSheet(COLOR_EDIT_INVERSIA_ACTIVATE);

        ui -> Apply -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> Exit -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
    }
}
