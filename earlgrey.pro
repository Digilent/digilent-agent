#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T11:00:21
#
#-------------------------------------------------

QT += core gui
QT += network

include(../QtWebApp/QtWebApp/httpserver/httpserver.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = earlgrey
TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/helloworldcontroller.cpp \
    src/listdatacontroller.cpp \
    src/requestmapper.cpp

HEADERS  += \
    src/mainwindow.h \
    src/helloworldcontroller.h \
    src/listdatacontroller.h \
    src/requestmapper.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    mainwindow.qrc

DISTFILES +=

OTHER_FILES += config.ini
