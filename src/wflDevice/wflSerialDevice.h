#ifndef WFLSERIALDEVICE_H
#define WFLSERIALDEVICE_H

#include <QObject>
#include <QThread>

#include "wflDevice.h"
#include "lib/digilent/qtSerial/serial.h"

class WflSerialDevice : public WflDevice{
    Q_OBJECT

public:
    WflSerialDevice(QString address, QObject *parent = 0);
    virtual ~WflSerialDevice();
    virtual bool isOpen();
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);

signals:
    void startFastWriteRead(QByteArray cmd, int delay, int timeout);
    void startSoftReset();

private slots:
    void onFastWriteReadResponse(QByteArray response);
    void onSoftResetResponse(bool success);

public slots:
    void release();
    virtual bool softReset();


private:
    Serial* serial;
    QByteArray data;
    bool softResetSuccess;
};


#endif // WFLSERIALDEVICE_H
