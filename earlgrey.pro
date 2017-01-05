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
target.path = /usr/bin

#config.path = /etc/skel
#config.files = waveforms-live-agent.ini

INSTALLS += target
#INSTALLS += config


TEMPLATE = app
CONFIG += qt


SOURCES += \
    src/httpServer/httpRouter.cpp \
    src/main.cpp \
    src/httpServer/debugController.cpp \
    src/mainWindow.cpp \
    src/httpClient/httpClient.cpp \    
    src/uartClient/uartClient.cpp \
    src/uartClient/uartInfo.cpp \
    src/core/comboBoxEventFilter.cpp \
    src/httpServer/agentConfigCtrl.cpp \
    src/core/agent.cpp \
    src/wflDevice/wflDevice.cpp \
    src/wflDevice/wflHttpDevice.cpp \
    src/wflDevice/wflUartDevice.cpp \
    src/core/utils/runGuard.cpp

HEADERS  += \    
    src/httpServer/httpRouter.h \
    src/mainWindow.h \
    src/httpClient/httpClient.h \   
    src/uartClient/uartClient.h \
    src/uartClient/uartInfo.h \
    src/core/comboBoxEventFilter.h \
    src/httpServer/agentConfigCtrl.h \
    src/httpServer/debugController.h \
    src/core/agent.h \
    src/wflDevice/wflDevice.h \
    src/wflDevice/wflUartDevice.h \
    src/core/utils/runGuard.h \
    src/wflDevice/wflHttpDevice.h

FORMS    += \
    mainWindow.ui

RESOURCES += \
    mainWindow.qrc

DISTFILES += \
#    waveforms-live-agent.ini

OTHER_FILES +=

#QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++
#QMAKE_LFLAGS += -static -lpthread -static-libgcc -static-libstdc++

