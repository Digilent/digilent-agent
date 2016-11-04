#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T11:00:21
#
#-------------------------------------------------

QT += core gui
QT += network

include(src/httpServer/core/httpserver.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = earlgrey
TEMPLATE = app


SOURCES += \
    src/httpServer/httpRouter.cpp \
    src/main.cpp \
    src/mainWindow.cpp \
    src/httpServer/debugController.cpp \
    src/httpServer/proxyController.cpp

HEADERS  += \    
    src/httpServer/httpRouter.h \
    src/mainWindow.h \
    src/httpServer/debugController.h \
    src/httpServer/proxyController.h

FORMS    += \
    mainWindow.ui

RESOURCES += \
    mainWindow.qrc

DISTFILES +=

OTHER_FILES += config.ini
