#include <QApplication>
#include <QTranslator>
#include <signal.h>

#include "mainwindow.h"

/*Главная функция программы, с которой начинается ее выполнение*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(":/mbdu_ru.qm"))
        app.installTranslator(&translator);

    MainWindow lMainWindow;
    lMainWindow.show();
    //lMainWindow.setWindowState(Qt::WindowFullScreen);

    return app.exec();
}
