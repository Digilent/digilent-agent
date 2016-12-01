#ifndef WFLUARTDEVICE_H
#define WFLUARTDEVICE_H

#include "wflDevice.h"
#include "../uartClient/uartClient.h"

class WflUartDevice : public WflDevice {
    Q_OBJECT

public:
    WflUartDevice(QString address);        
    virtual void execCommand(QByteArray cmd);
    virtual void write(QByteArray cmd);

signals:

public slots:
    void onUartComplete(QByteArray reply);
    void onUartTimeout(QByteArray message);

private:
    QString address;
    QByteArray reply;
    UartClient* uartClient;

};

#endif // WFLUARTDEVICE_H
