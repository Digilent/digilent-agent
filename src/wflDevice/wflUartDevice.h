#ifndef WFLUARTDEVICE_H
#define WFLUARTDEVICE_H

#include "wflDevice.h"
#include "../uartClient/uartClient.h"

class WflUartDevice : public WflDevice {
    Q_OBJECT

public:
    WflUartDevice(QString address);        
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);
    virtual void softReset();

signals:


public slots:
    void onUartExecCmdComplete(QByteArray reply);
    void onUartTimeout(QByteArray message);
    void onWriteReadResponse(QByteArray reply);

private:
    QString address;
    QByteArray reply;
    UartClient* uartClient;

};

#endif // WFLUARTDEVICE_H
