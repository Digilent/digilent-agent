#ifndef OSDEVICE_H
#define OSDEVICE_H

#include <QObject>

class OsDevice : public QObject
{
    Q_OBJECT

public:
    explicit OsDevice(QObject *parent = 0);

    //Functions
    virtual void execCommand(QString cmd);

    //Variables
    QString name;
    QString deviceType;

signals:    
    void execCommandComplete(QString response);

public slots:

private:

};

#endif // OSDEVICE_H
