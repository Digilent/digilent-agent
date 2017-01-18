#ifndef WFLSERIALDEVICE_H
#define WFLSERIALDEVICE_H

#include <QObject>

#include "wflDevice.h"
#include "lib/digilent/qtSerial/serial.h"

class WflSerialDevice : public WflDevice{
    Q_OBJECT

public:
    WflSerialDevice(QString address);
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);
    virtual void softReset();

signals:

public slots:

private:
    Serial serial;

};


#endif // WFLSERIALDEVICE_H
