#ifndef WFLDEVICE_H
#define WFLDEVICE_H

#include <QObject>
#include <QThread>

class WflDevice : public QObject
{
    Q_OBJECT

public:
    explicit WflDevice(QObject *parent = 0);
    virtual ~WflDevice();

    //Functions    
    virtual QByteArray writeRead(QByteArray cmd) = 0;
    virtual void execCommand(QByteArray cmd) = 0;
    virtual bool isOpen() = 0;    

    //Variables
    QString name;
    QString deviceType;

signals:    
    void execCommandComplete(QByteArray response);
    void writeReadComplete();
    void fastWriteReadComplete();
    void softResetComplete();
    void releaseComplete();

public slots:
    virtual void release() = 0;
    virtual bool softReset() = 0;

private:

};

#endif // WFLDEVICE_H
