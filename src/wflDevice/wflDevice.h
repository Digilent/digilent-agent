#ifndef WFLDEVICE_H
#define WFLDEVICE_H

#include <QObject>

class WflDevice : public QObject
{
    Q_OBJECT

public:
    explicit WflDevice(QObject *parent = 0);
    virtual ~WflDevice();

    //Functions    
    virtual void execCommand(QByteArray cmd);
    virtual QByteArray writeRead(QByteArray cmd) = 0;
    virtual bool softReset() = 0;
    virtual bool isOpen() = 0;

    //Variables
    QString name;
    QString deviceType;

signals:    
    void execCommandComplete(QByteArray response);
    void writeReadComplete();

public slots:

private:

};

#endif // WFLDEVICE_H
