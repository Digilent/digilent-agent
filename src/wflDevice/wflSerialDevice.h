#ifndef WFLSERIALDEVICE_H
#define WFLSERIALDEVICE_H

#include <QObject>

#include "wflDevice.h"
#include "lib/digilent/qtSerial/serial.h"

class WflSerialDevice : public WflDevice{
    Q_OBJECT

public:
    WflSerialDevice(QString address);
    virtual ~WflSerialDevice();
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);
    virtual bool softReset();
    virtual bool isOpen();

signals:

public slots:
    virtual void release();

private:
    Serial serial;

};


#endif // WFLSERIALDEVICE_H
