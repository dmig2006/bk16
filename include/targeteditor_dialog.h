#ifndef TARGETEDITOR_H
#define TARGETEDITOR_H

#include <QMutex>
#include <QDialog>
#include <QListWidgetItem>
#include <QCloseEvent>
#include <QChar>

#include "aimlist.h"
#include "controllermanager.h"

namespace Ui {
    class TargetEditorDialog;
}

class TargetEditorDialog : public QDialog
{
    Q_OBJECT

    Ui::TargetEditorDialog *ui;
    ControllerManager *mControllerManager;
    QList<Aim*> currentAims;

    QColor colorChangeBackground;
    QColor colorChangeText;

    QColor colorUnchangeBackground;
    QColor colorUnchangeText;

    void initList();
    void saveList();
    void addItem(Aim *pAim);
    void setItem(int row, Aim *pAim);
    void editItem(int row, Aim *pAim);

public:
    TargetEditorDialog(QWidget *parent = 0);
    ~TargetEditorDialog();
    void setControllerManager(ControllerManager *pControllerManager);
    QListWidgetItem* ParsingAim(int row, Aim *pAim);
    void colorInversia(bool pFlag);

private:
    bool flagInversia;

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *);

Q_SLOT
    void on_apply_Button_clicked();
Q_SLOT
    void on_cancel_Button_clicked();

private slots:
    void on_selected_active_Button_clicked();
    void on_selected_deactive_Button_clicked();
    void on_active_selected_Button_clicked();
    void on_active_all_Button_clicked();
    void on_deactive_all_Button_clicked();
    void on_delete_selected_Button_clicked();
    void on_delete_all_Button_clicked();
    void on_deactive_selected_Button_clicked();
    void on_listWidget_clicked();
};

#endif // TARGETEDITOR_H
