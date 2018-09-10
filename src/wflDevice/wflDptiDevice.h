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
    WflDptiDevice(QString deviceName, QObject *parent = 0);
    virtual ~WflDptiDevice();
    virtual bool isOpen();
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd);

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
    HIF *deviceHandle;
    QByteArray data;
    bool softResetSuccess;
};


#endif // WFLDPTIDEVICE_H
