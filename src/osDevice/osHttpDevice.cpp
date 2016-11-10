#include "osHttpDevice.h"

OsHttpDevice::OsHttpDevice(QUrl _url) {
    qDebug("OsHttpDevice Constructor");
    name = "HTTP Device";
    url = _url;
}

void OsHttpDevice::execCommand(QString cmd) {
    qDebug("OsHttpDevice::execCommand()");
    qDebug(cmd.toUtf8());

    emit commandComplete(QString("Response"));
}
