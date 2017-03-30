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

wwwRoot.path = /usr/share/digilent-agent/www
wwwRoot.files = www/*

INSTALLS += target
INSTALLS += wwwRoot

TEMPLATE = app
CONFIG += qt

#CONFIG+=console

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

unix:SOURCES += \
    lib/zlib-1.2.11/adler32.c \
    lib/zlib-1.2.11/compress.c \
    lib/zlib-1.2.11/crc32.c \
    lib/zlib-1.2.11/deflate.c \
    lib/zlib-1.2.11/gzclose.c \
    lib/zlib-1.2.11/gzlib.c \
    lib/zlib-1.2.11/gzread.c \
    lib/zlib-1.2.11/gzwrite.c \
    lib/zlib-1.2.11/infback.c \
    lib/zlib-1.2.11/inffast.c \
    lib/zlib-1.2.11/inflate.c \
    lib/zlib-1.2.11/inftrees.c \
    lib/zlib-1.2.11/trees.c \
    lib/zlib-1.2.11/uncompr.c \
    lib/zlib-1.2.11/zutil.c \
    lib/quazip-0.7/quazip/JlCompress.cpp \
    lib/quazip-0.7/quazip/qioapi.cpp \
    lib/quazip-0.7/quazip/quaadler32.cpp \
    lib/quazip-0.7/quazip/quacrc32.cpp \
    lib/quazip-0.7/quazip/quagzipfile.cpp \
    lib/quazip-0.7/quazip/quaziodevice.cpp \
    lib/quazip-0.7/quazip/quazip.cpp \
    lib/quazip-0.7/quazip/quazipdir.cpp \
    lib/quazip-0.7/quazip/quazipfile.cpp \
    lib/quazip-0.7/quazip/quazipfileinfo.cpp \
    lib/quazip-0.7/quazip/quazipnewinfo.cpp \
    lib/quazip-0.7/quazip/zip.c \
    lib/quazip-0.7/quazip/unzip.c

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

unix:HEADERS += \
    lib/zlib-1.2.11/crc32.h \
    lib/zlib-1.2.11/deflate.h \
    lib/zlib-1.2.11/gzguts.h \
    lib/zlib-1.2.11/inffast.h \
    lib/zlib-1.2.11/inffixed.h \
    lib/zlib-1.2.11/inflate.h \
    lib/zlib-1.2.11/inftrees.h \
    lib/zlib-1.2.11/trees.h \
    lib/zlib-1.2.11/zconf.h \
    lib/zlib-1.2.11/zlib.h \
    lib/zlib-1.2.11/zutil.h \
    lib/quazip-0.7/quazip/crypt.h \
    lib/quazip-0.7/quazip/ioapi.h \
    lib/quazip-0.7/quazip/JlCompress.h \
    lib/quazip-0.7/quazip/quaadler32.h \
    lib/quazip-0.7/quazip/quachecksum32.h \
    lib/quazip-0.7/quazip/quacrc32.h \
    lib/quazip-0.7/quazip/quagzipfile.h \
    lib/quazip-0.7/quazip/quaziodevice.h \
    lib/quazip-0.7/quazip/quazip_global.h \
    lib/quazip-0.7/quazip/quazip.h \
    lib/quazip-0.7/quazip/quazipdir.h \
    lib/quazip-0.7/quazip/quazipfile.h \
    lib/quazip-0.7/quazip/quazipfileinfo.h \
    lib/quazip-0.7/quazip/quazipnewinfo.h \
    lib/quazip-0.7/quazip/unzip.h \
    lib/quazip-0.7/quazip/zip.h

FORMS    += \
    mainWindow.ui

RESOURCES += \
    mainWindow.qrc

RC_FILE = digilent-agent.rc

DISTFILES += \
#    digilent-agent.ini
    lib/digilent/qtSerial/.gitignore \
    lib/digilent/qtSerial/LICENSE \
    lib/digilent/qtSerial/README.md \
    lib/digilent/pgm/.gitignore \
    lib/digilent/pgm/LICENSE

OTHER_FILES +=

#Libs
#INCLUDEPATH += $$PWD/lib/zlib-1.2.8
#LIBS += -L$$PWD/lib/zlib-1.2.8 -lz
INCLUDEPATH += $$PWD/lib/quazip-0.7/quazip
win32:LIBS += -L$$PWD/lib/quazip-0.7/quazip/release -lquazip


#QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++
#QMAKE_LFLAGS += -static -lpthread -static-libgcc -static-libstdc++

