TEMPLATE     = app
TARGET       = mbdu

DESTDIR      = ./bin
VPATH       += ./gui ./include ./src ./resources ./i10n
INCLUDEPATH += ./include
LIBS        += -lasound
MOC_DIR      = ./build/moc
OBJECTS_DIR  = ./build/objects
UI_DIR       = ./build/ui
RCC_DIR      = ./build/res

QT += gui core
QT += network
isEqual(QT_MAJOR_VERSION, 5) {
	QT += widgets
}

CONFIG += release

HEADERS += action.h \
           actionlist.h \
           adjustment_dialog.h \
           aim.enum.h \
           aim.h \
           aimlist.h \
           ammunition_dialog.h \
           automodetype_dialog.h \
           ballistics.h \
           beeper.h \
           beeperthread.h \
           camera.h \
           camera_error.h \
           cfgfiles.h \
           cfgmanager.h \
           client.h \
           comport.h \
           controldevice.h \
           controllermanager.h \
           crc8.h \
           devicethread.h \
           joystick.h \
           mainwindow.h \
           optwidget.h \
           plathormdir.h \
           supportmanager.h \
           tables.enum.h \
           tcpclient.h \
           tinystr.h \
           tinyxml.h \
           tracking.h \
           video_client.h \
           videowidget.h \
           view.enum.h \
           weapon.enum.h \
           qsocketthred.h \
           qsimpleserver.h \
           pid.h \
           canad.h \
           burstlength_dialog.h \
           soundsignal_dialog.h \
           targeteditor_dialog.h \
           tracking_zala.h

FORMS += adjustment_dialog.ui \
         ammunition_dialog.ui \
         automodetype_dialog.ui \
         mainwindow.ui \
         optwidget.ui \
         plathormdir.ui \
         soundsignal_dialog.ui \
         videowidget.ui \
         pid.ui \
         burstlength_dialog.ui \
         targeteditor_dialog.ui

SOURCES += actionlist.cpp \
           adjustment_dialog.cpp \
           aimlist.cpp \
           ammunition_dialog.cpp \
           automodetype_dialog.cpp \
           ballistics.cpp \
           beeper.cpp \
           beeperthread.cpp \
           camera.cpp \
           cfgmanager.cpp \
           client.cpp \
           comport.cpp \
           controldevice.cpp \
           controllermanager.cpp \
           crc8.cpp \
           devicethread.cpp \
           joystick.cpp \
           main.cpp \
           mainwindow.cpp \
           optwidget.cpp \
           plathormdir.cpp \
           supportmanager.cpp \
           tcpclient.cpp \
           tinystr.cpp \
           tinyxml.cpp \
           tinyxmlerror.cpp \
           tinyxmlparser.cpp \
           tracking.cpp \
           video_client.cpp \
           videowidget.cpp \
           canad.cpp \
           pid.cpp \
           qsimpleserver.cpp \
           qsocketthred.cpp \
           burstlength_dialog.cpp \
           soundsignal_dialog.cpp \
           targeteditor_dialog.cpp \
           tracking_zala.cpp

RESOURCES = mbdu_res.qrc

TRANSLATIONS = l10n/mbdu_ru.ts
