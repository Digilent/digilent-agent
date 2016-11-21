#ifndef UARTCLIENT_H
#define UARTCLIENT_H

//QT core includes
#include <QThread>
#include <QMutex>
#include <QWaitCondition>


class UartClient : public QThread
{
    Q_OBJECT

public:
    UartClient(QObject *parent = 0);
    ~UartClient();

    void writeRead(const QString &portName, int waitTimeout, const QString &dataToSend);
    void run();

signals:
    void response(QString response);
    void error(QString response);
    void timeout(QString response);

private:
    QString portName;
    QString dataToSend;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};

#endif // UARTCLIENT_H

