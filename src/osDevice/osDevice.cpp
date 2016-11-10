#include "osDevice.h"

OsDevice::OsDevice(QObject *parent) : QObject(parent)
{
    qDebug("OsDevice Constructor");
    name = "Unknown Device";
}

void OsDevice::execCommand(QString cmd){
    qDebug("OsDevice::execCommand()");
}
