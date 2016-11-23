#ifndef OSUARTDEVICE_H
#define OSUARTDEVICE_H

#include "osDevice.h"
#include "../uartClient/uartClient.h"

class OsUartDevice : public OsDevice {
    Q_OBJECT

public:
    OsUartDevice(QString address);
    virtual void execCommand(QByteArray cmd);

signals:

public slots:
    void onUartComplete(QByteArray reply);
    void onUartTimeout(QByteArray message);

private:
    QString address;
    QByteArray reply;
    UartClient* uartClient;

};

#endif // OSUARTDEVICE_H
