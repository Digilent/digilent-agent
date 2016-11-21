#ifndef UARTINFO_H
#define UARTINFO_H

#include <QSerialPortInfo>
#include <QList>

class UartInfo
{
public:
    UartInfo();

    //Functions
    void refreshPortInfo();

    //Variables
    QList<QSerialPortInfo> ports;

private:

};

#endif // UARTINFO_H
