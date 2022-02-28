#include "targeteditor_dialog.h"
#include "ui_targeteditor_dialog.h"

/*Конструктор класса*/
TargetEditorDialog::TargetEditorDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::TargetEditorDialog)
{
    ui->setupUi(this);
    flagInversia = true;
    initList();
}

/*Деструктор класса*/
TargetEditorDialog::~TargetEditorDialog()
{
    delete ui;
}

/*получение указателя на ControllerManager*/
void TargetEditorDialog::setControllerManager(ControllerManager *pControllerManager)
{
    mControllerManager = pControllerManager;
}

/*разбор цели и формирование сторки списка целей*/
QListWidgetItem* TargetEditorDialog::ParsingAim(int row, Aim *pAim)
{
    QListWidgetItem *newItem = new QListWidgetItem;
    if (pAim->mState)
        newItem->setCheckState(Qt::Checked);
    else
        newItem->setCheckState(Qt::Unchecked);
    QString lId = QString::number(row + 1);
    QString lnumTarget = QString::number(pAim->mTarget);
    QString lActive;
    if (pAim->mActive)
        lActive = tr("active");
    else
        lActive = tr("inactive");
    float angle = static_cast<float>(pAim->mPositionX)
                   / DEGREE_TO_CENTIDEGREE;
    if (angle < 0.0)
        angle += 360.0;

    QString lnumPosX = QString::number(angle, 0, 2);
    QString lnumPosY = QString::number(-(float)pAim->mPositionY
                                       / DEGREE_TO_CENTIDEGREE, 0, 2);
    QString lnumRange = QString::number(pAim->mRange);
    newItem->setText(lId + tr(" Target ") + lnumTarget + ", " + lActive
                     + ", " + lnumPosX + QChar(0x00b0) + ", " + lnumPosY + QChar(0x00b0) + tr(", ")
                     + lnumRange + tr(" m"));
    return newItem;
}

void TargetEditorDialog::colorInversia(bool pFlag)
{
    flagInversia = pFlag;
    if (pFlag)
    {
        ui ->listWidget->setStyleSheet(COLOR_LIST_NORMA);

        ui -> apply_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> cancel_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> selected_active_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> selected_deactive_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> active_selected_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> deactive_selected_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> active_all_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> deactive_all_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> delete_selected_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);
        ui -> delete_all_Button -> setStyleSheet(COLOR_BUTTON_NORMA_ACTIVATE);

        colorChangeBackground = QColor(0x9CC75B);
        colorChangeText =QColor(0x610404);

        colorUnchangeBackground = QColor(0xFFFFFF);
        colorUnchangeText =QColor(0x610404);
    }
    else
    {
        ui ->listWidget->setStyleSheet(COLOR_LIST_INVERSIA);

        ui -> apply_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> cancel_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> selected_active_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> selected_deactive_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> active_selected_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> deactive_selected_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> active_all_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> deactive_all_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> delete_selected_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);
        ui -> delete_all_Button -> setStyleSheet(COLOR_BUTTON_INVERSIA_ACTIVATE);

        colorChangeBackground = QColor(0x363336);
        colorChangeText =QColor(0xFFFFFF);

        colorUnchangeBackground = QColor(0x989197);
        colorUnchangeText =QColor(0xECD2D2);
    }
}

/*переопределение обработчика событий изменения состояния*/
void TargetEditorDialog::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*обработчик события closeEvent*/
void TargetEditorDialog::closeEvent(QCloseEvent *event)
{
    mControllerManager->setPause(false);
    event->accept();
}

/*отрисовка списка целей*/
void TargetEditorDialog::paintEvent(QPaintEvent *)
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i)
        if (flagInversia)
        {
            if(ui->listWidget->item(i)->checkState() == Qt::Checked)
            {
                ui->listWidget->item(i)->setBackground(colorChangeBackground);
                ui->listWidget->item(i)->setTextColor(colorChangeText);
            }
            else
            {
                ui->listWidget->item(i)->setBackground(colorUnchangeBackground);
                ui->listWidget->item(i)->setTextColor(colorUnchangeText);
            }
        }
    else
        {
            if(ui->listWidget->item(i)->checkState() == Qt::Checked)
            {
                ui->listWidget->item(i)->setBackground(colorChangeBackground);
                ui->listWidget->item(i)->setTextColor(colorChangeText);
            }
            else
            {
                ui->listWidget->item(i)->setBackground(colorUnchangeBackground);
                ui->listWidget->item(i)->setTextColor(colorUnchangeText);
            }
        }
}

/*инициализация списка целей*/
void TargetEditorDialog::initList()
{
    int size = AimList::getSingletonPtr()->size();
    for (int i = 0; i < size; ++i) {
        Aim *lAim = AimList::getSingletonPtr()->get(i);
        setItem(i, lAim);
        currentAims.push_back(lAim);
    }
    this->repaint();
}

/*сохранение списка целей*/
void TargetEditorDialog::saveList()
{
    AimList::getSingletonPtr()->clear();
    int size = currentAims.size();
    for (int i = 0; i < size; ++i) {
        if (ui->listWidget->item(i)->checkState() == Qt::Checked)
            currentAims[i]->mState = true;
        else
            currentAims[i]->mState = false;
        AimList::getSingletonPtr()->push(currentAims[i]);
    }
    size = AimList::getSingletonPtr()->size();
    mControllerManager->setDisplayLastAim(AimList::getSingletonPtr()->get(size - 1));
}

/*добавление елемента списка целей*/
void TargetEditorDialog::setItem(int row, Aim *pAim)
{
    if (pAim) {
        QListWidgetItem *newItem = ParsingAim(row, pAim);
        ui->listWidget->insertItem(row, newItem);
    }
}

/*редактировать строку списка целей*/
void TargetEditorDialog::editItem(int row, Aim *pAim)
{
    if (pAim)
        ui->listWidget->item(row)->setText(ParsingAim(row,pAim)->text());
}

/*обработчик кнопки применить*/
void TargetEditorDialog::on_apply_Button_clicked()
{
    saveList();
    currentAims.clear();
    mControllerManager->setPause(false);
    close();
}

/*обработчик кнопки отменить*/
void TargetEditorDialog::on_cancel_Button_clicked()
{
    mControllerManager->setPause(false);
    close();
}

/*обработчик кнопки выбрать активные цели*/
void TargetEditorDialog::on_selected_active_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i) {
        if (currentAims[i]->mActive)
            ui->listWidget->item(i)->setCheckState(Qt::Checked);
        if (!currentAims[i]->mActive)
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
    }
    update();
}

/*обработчик кнопки выбрать неактивные цели*/
void TargetEditorDialog::on_selected_deactive_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i) {
        if (currentAims[i]->mActive)
            ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
        if (!currentAims[i]->mActive)
            ui->listWidget->item(i)->setCheckState(Qt::Checked);
    }
    update();
}

/*обработчик кнопки активировать выбранные цели*/
void TargetEditorDialog::on_active_selected_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i)
        if(ui->listWidget->item(i)->checkState() == Qt::Checked){
            currentAims[i]->mActive = true;
            editItem(i, currentAims[i]);
        }
    update();
}

/*обработчик кнопки активировать все цели*/
void TargetEditorDialog::on_active_all_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i) {
        currentAims[i]->mActive = true;
        editItem(i, currentAims[i]);
    }
    update();
}

/*обработчик кнопки деактивировать все цели*/
void TargetEditorDialog::on_deactive_all_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i) {
        currentAims[i]->mActive = false;
        editItem(i, currentAims[i]);
    }
    update();
}

/*обработчик кнопки удалить выбранные цели*/
void TargetEditorDialog::on_delete_selected_Button_clicked()
{
    int size = currentAims.size();
    QList<Aim*> tempList;
    for (int i = 0; i < size; ++i)
        if (ui->listWidget->item(i)->checkState() == Qt::Unchecked)
            tempList.push_back(currentAims[i]);
    ui->listWidget->clear();
    currentAims.clear();
    currentAims = tempList;
    size = currentAims.size();
    for (int i = 0; i < size; ++i)
        setItem(i, currentAims.at(i));
    update();
}

/*обработчик кнопки удалить все цели*/
void TargetEditorDialog::on_delete_all_Button_clicked()
{
    ui->listWidget->clear();
    currentAims.clear();
    update();
}

/*обработчик кнопки деактивировать выбранные цели*/
void TargetEditorDialog::on_deactive_selected_Button_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i)
        if (ui->listWidget->item(i)->checkState() == Qt::Checked) {
            currentAims[i]->mActive = false;
            editItem(i, currentAims[i]);
        }
    update();
}

/*обработчик выбора элемента списка*/
void TargetEditorDialog::on_listWidget_clicked()
{
    int size = currentAims.size();
    for (int i = 0; i < size; ++i)
        if(ui->listWidget->item(i)->isSelected()) {
            if (ui->listWidget->item(i)->checkState() == Qt::Unchecked) {
                ui->listWidget->item(i)->setCheckState(Qt::Checked);
                currentAims[i]->mState = true;
            } else {
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
                currentAims[i]->mState = false;
            }
        }
    update();
}
