#ifndef OSHTTPDEVICE_H
#define OSHTTPDEVICE_H

//QT core includes
#include <QUrl>

#include "osDevice.h"

class OsHttpDevice : public OsDevice {

public:
    OsHttpDevice(QUrl _url);
    virtual void execCommand(QString cmd);

signals:
    //virtual void commandComplete(QString response);

private:
    QUrl url;
};

#endif // OSHTTPDEVICE_H
