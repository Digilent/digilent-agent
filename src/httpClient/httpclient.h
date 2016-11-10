#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

//QT core includes
#include <QNetworkReply>

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = 0);
    void get(QString url);   

signals:
    void complete(QNetworkReply *reply);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* networkManager;   
};

#endif // HTTPCLIENT_H
