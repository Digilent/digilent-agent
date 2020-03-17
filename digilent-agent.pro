#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T11:00:21
#
#-------------------------------------------------

QT += core gui
QT += network
QT += serialport
QT += concurrent

include(src/httpServer/core/httpserver.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = digilent-agent
target.path = /usr/bin

wwwRoot.path = /usr/share/digilent/digilent-agent/www
wwwRoot.files = www/*

INSTALLS += target
INSTALLS += wwwRoot

TEMPLATE = app
CONFIG += qt

CONFIG(debug, debug|release) {
    DEFINES += AGENT_BUILD_NOTE=\\\"DEBUG\\\"
    CONFIG+=console
}


SOURCES += \
    src/httpServer/httpRouter.cpp \
    src/main.cpp \
    src/httpServer/debugController.cpp \
    src/mainWindow.cpp \
    src/core/comboBoxEventFilter.cpp \
    src/httpServer/agentConfigCtrl.cpp \
    src/core/agent.cpp \
    src/wflDevice/wflDevice.cpp \
    src/core/utils/runGuard.cpp \
    lib/digilent/qtSerial/serial.cpp \
    src/wflDevice/wflSerialDevice.cpp \
    lib/digilent/pgm/digilentPgm.cpp \
    lib/digilent/pgm/pgmBlock.cpp \
    lib/digilent/qtHttp/httpClient.cpp \



HEADERS  += \
    src/httpServer/httpRouter.h \
    src/mainWindow.h \
    src/core/comboBoxEventFilter.h \
    src/httpServer/agentConfigCtrl.h \
    src/httpServer/debugController.h \
    src/core/agent.h \
    src/wflDevice/wflDevice.h \
    src/core/utils/runGuard.h \
    lib/digilent/qtSerial/serial.h \
    src/wflDevice/wflSerialDevice.h \
    lib/digilent/pgm/digilentPgm.h \
    lib/digilent/pgm/pgmBlock.h \
    lib/digilent/qtHttp/httpClient.h \

FORMS    += \
    mainWindow.ui

RESOURCES += \
    mainWindow.qrc

RC_FILE = digilent-agent.rc
ICON = images/icon.icns

DISTFILES += \
#    digilent-agent.ini
    lib/digilent/qtSerial/.gitignore \
    lib/digilent/qtSerial/LICENSE \
    lib/digilent/qtSerial/README.md \
    lib/digilent/pgm/.gitignore \
    lib/digilent/pgm/LICENSE

OTHER_FILES +=

#QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++
#QMAKE_LFLAGS += -static -lpthread -static-libgcc -static-libstdc++

