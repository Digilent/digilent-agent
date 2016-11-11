#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

//QT core includes
#include <QNetworkReply>
#include <QUrl>

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = 0);
    void get(QUrl url);
    void post(QUrl url, QString body);
    void post(QNetworkRequest request, QString body);

signals:
    void complete(QNetworkReply *reply);

private slots:
    void onRequestComplete(QNetworkReply *reply);

private:
    QNetworkAccessManager* networkManager;   
};

#endif // HTTPCLIENT_H
