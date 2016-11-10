#include "httpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void HttpClient::get(QString url){
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void HttpClient::onFinished(QNetworkReply *reply) {
    emit complete(reply);
}
