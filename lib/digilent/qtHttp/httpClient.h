#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class HttpClient : public QObject
{
    Q_OBJECT
public:
    HttpClient(QObject *parent = 0);
    QByteArray get(QUrl url);
    //static QByteArray getBody(QUrl url);

signals:
    void requestComplete();

public slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    QByteArray replyBody;
};

#endif // HTTPCLIENT_H
