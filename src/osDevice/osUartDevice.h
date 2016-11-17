#ifndef OSUARTDEVICE_H
#define OSUARTDEVICE_H

#include "osDevice.h"
#include "../uartClient/uartClient.h"

class OsUartDevice : public OsDevice {
    Q_OBJECT

public:
    OsUartDevice(QString address);
    virtual void execCommand(QString cmd);

signals:

public slots:
    void onUartComplete(QString reply);
    void onUartTimeout(QString message);

private:
    QString address;
    QString reply;
    UartClient* uartClient;

};

#endif // OSUARTDEVICE_H
