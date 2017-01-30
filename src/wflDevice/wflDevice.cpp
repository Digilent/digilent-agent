#include "wflDevice.h"

#include <QDebug>

WflDevice::WflDevice(QObject *parent) : QObject(parent)
{
    qDebug("WflDevice Constructor");
    deviceType = "Generic OpenScope Device";
    name = "Unknown Device";
}

void WflDevice::execCommand(QByteArray cmd){
    qDebug() << "OsDevice::execCommand()" << cmd;
}
