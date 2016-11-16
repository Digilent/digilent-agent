#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

//QT core includes
#include <QNetworkReply>
#include <QUrl>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QObject>

class HttpClient : public QThread
{
    Q_OBJECT
public:
    HttpClient(QObject *parent = 0);
    ~HttpClient();

    void get(QUrl url);
    void post(QNetworkRequest request, QString body);
    void run() Q_DECL_OVERRIDE;

signals:
    void complete(QNetworkReply *reply);
    void taskComplete();

private slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    QNetworkRequest request;
    QUrl url;
    QString body;
    QString method;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
};

#endif // HTTPCLIENT_H
