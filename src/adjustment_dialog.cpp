#include "adjustment_dialog.h"
#include "ui_adjustment_dialog.h"

/*Конструктор класса*/
AdjustmentDialog::AdjustmentDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::AdjustmentDialog)
{
    ui->setupUi(this);
    move(500, 30);
}

/*Деструктор класса*/
AdjustmentDialog::~AdjustmentDialog()
{
    delete ui;
}

/*получение данных предыдущей юстировки*/
void AdjustmentDialog::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
    mPosX = mControllerManager->getJustX();
    mPosY = mControllerManager->getJustY();
}

void AdjustmentDialog::colorInversia(bool pInversia)
{
    if (pInversia)
    {
        ui -> line -> setStyleSheet(COLOR_LINE_NORMA);
        ui -> line_2 -> setStyleSheet(COLOR_LINE_NORMA);
        ui -> line_3 -> setStyleSheet(COLOR_LINE_NORMA);
        ui -> line_4 -> setStyleSheet(COLOR_LINE_NORMA);

        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> centerButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> leftButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> rightButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> upButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> downButton -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

    }
    else
    {
        ui -> line -> setStyleSheet(COLOR_LINE_INVERSIA);
        ui -> line_2 -> setStyleSheet(COLOR_LINE_INVERSIA);
        ui -> line_3 -> setStyleSheet(COLOR_LINE_INVERSIA);
        ui -> line_4 -> setStyleSheet(COLOR_LINE_INVERSIA);

        ui -> applyButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancelButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> centerButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> leftButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> rightButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> upButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> downButton -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
    }
}

/*переопределение обработчика событий изменения состояния*/
void AdjustmentDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        //ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*обработчик события closeEvent*/
void AdjustmentDialog::closeEvent(QCloseEvent *event)
{
    mControllerManager->setPause(false);
    event->accept();
}

/*обработчик кнопки вверх*/
void AdjustmentDialog::on_upButton_pressed()
{
    long pJustX = mControllerManager->getJustX();
    long pJustY = mControllerManager->getJustY();
    pJustY -= 1;
    mControllerManager->setJust(pJustX, pJustY);
}

/*обработчик кнопки влево*/
void AdjustmentDialog::on_leftButton_pressed()
{
    long pJustX = mControllerManager->getJustX();
    long pJustY = mControllerManager->getJustY();
    pJustX += 1;
    mControllerManager->setJust(pJustX, pJustY);
}

/*обработчик кнопки вниз*/
void AdjustmentDialog::on_downButton_pressed()
{
    long pJustX = mControllerManager->getJustX();
    long pJustY = mControllerManager->getJustY();
    pJustY += 1;
    mControllerManager->setJust(pJustX, pJustY);
}

/*обработчик кнопки вправо*/
void AdjustmentDialog::on_rightButton_pressed()
{
    long pJustX = mControllerManager->getJustX();
    long pJustY = mControllerManager->getJustY();
    pJustX -= 1;
    mControllerManager->setJust(pJustX, pJustY);
}

/*обработчик кнопки центр*/
void AdjustmentDialog::on_centerButton_clicked()
{
    mControllerManager->setJust(0, 0);
}

/*обработчик кнопки применить*/
void AdjustmentDialog::on_applyButton_released()
{
    mControllerManager->saveJust();
    mControllerManager->setPause(false);
    close();
}

/*обработчик кнопки отменить*/
void AdjustmentDialog::on_cancelButton_clicked()
{
    mControllerManager->setJust(mPosX, mPosY);
    mControllerManager->setPause(false);
    close();
}

