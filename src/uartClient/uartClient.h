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

    void writeRead(const QString &portName, int waitTimeout, const QByteArray &dataToSend);
    void run();

signals:
    void response(QByteArray response);
    void error(QByteArray response);
    void timeout(QByteArray response);

private:
    QString portName;
    QByteArray dataToSend;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};

#endif // UARTCLIENT_H

