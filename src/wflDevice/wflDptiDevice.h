#ifndef WFLDPTIDEVICE_H
#define WFLDPTIDEVICE_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include "wflDevice.h"

//OS specific includes and config
#ifdef _WIN32
    #include <windows.h>
#elif __linux__

#elif __APPLE__

#endif

#include "lib/digilent/adept/include/dpcdecl.h"
#include "lib/digilent/adept/include/dmgr.h"
#include "lib/digilent/adept/include/dpti.h"

class WflDptiDevice : public WflDevice{
    Q_OBJECT

public:
    WflDptiDevice(QString deviceName, QString deviceSerial, QObject *parent = 0);
    virtual ~WflDptiDevice();
    virtual bool isOpen();
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray data);

signals:
    //void startFastWriteRead(QByteArray cmd, int delay, int timeout);
    //void startSoftReset();

private slots:
    //void onFastWriteReadResponse(QByteArray response);
    //void onSoftResetResponse(bool success);

public slots:
    void release();
    virtual bool softReset();


private:
    HIF deviceHandle = 0;
    QByteArray data;
    bool softResetSuccess;

    //Write
    bool write(const char *data, int numBytes);                         //Write data to the port.  Return true on success
    bool write(QByteArray data);                                        //Write data to the port.  Return true on success
    QByteArray dipWriteRead(QByteArray data, int delay, int timeout);

    //Read
    QByteArray read();                                                  //Read all available bytes from the buffer
    unsigned long bytesAvailable();//                                   //Return the number of bytes available in the buffer
    bool waitForBytesAvailable(unsigned long numBytes, int timeout);    //Wait for the specified number of bytes to be avaialable in the buffer or a timeout
};


#endif // WFLDPTIDEVICE_H
