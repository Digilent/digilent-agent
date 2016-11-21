#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T11:00:21
#
#-------------------------------------------------

QT += core gui
QT += network
QT += serialport

include(src/httpServer/core/httpserver.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = earlgrey
TEMPLATE = app


SOURCES += \
    src/httpServer/httpRouter.cpp \
    src/main.cpp \
    src/httpServer/debugController.cpp \
    src/mainWindow.cpp \
    src/httpClient/httpClient.cpp \
    src/osDevice/osDevice.cpp \
    src/osDevice/osHttpDevice.cpp \
    src/osDevice/osUartDevice.cpp \
    src/uartClient/uartClient.cpp \
    src/uartClient/uartInfo.cpp

HEADERS  += \    
    src/httpServer/httpRouter.h \
    src/httpServer/debugController.h \
    src/mainWindow.h \
    src/httpClient/httpClient.h \
    src/osDevice/osDevice.h \
    src/osDevice/osHttpDevice.h \
    src/osDevice/osUartDevice.h \
    src/uartClient/uartClient.h \
    src/uartClient/uartInfo.h

FORMS    += \
    mainWindow.ui

RESOURCES += \
    mainWindow.qrc

DISTFILES +=

OTHER_FILES += config.ini
