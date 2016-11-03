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
    src/mainwindow.cpp

HEADERS  += \
    src/mainwindow.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    mainwindow.qrc

DISTFILES +=

OTHER_FILES += config.ini
