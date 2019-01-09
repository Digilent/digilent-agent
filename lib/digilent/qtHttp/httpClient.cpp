#include "httpClient.h"

#include <QEventLoop>
#include <QNetworkReply>

HttpClient::HttpClient(QObject *parent) : QObject(parent) {

}

//Perform an HTTP GET on the specified URL.  This function blocks until the request is complete or the specified timeout.
//If the reqeust completes the response is returned, null is returned otherwise.
//TODO - Add timeout
QByteArray HttpClient::get(QUrl url) {
    QNetworkAccessManager qnam;
    QEventLoop loop;

    //connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestFinished(QNetworkReply*)));
    QNetworkRequest request(url);
    QNetworkReply *reply = qnam.get(request);

    //Wait for signal that proxy call has returned
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    //Return response body
    return reply->readAll();
}

void HttpClient::onRequestFinished(QNetworkReply *reply) {
    QList<QByteArray> headers = reply->rawHeaderList();
    this->replyBody = reply->readAll();
    emit requestComplete();
}
