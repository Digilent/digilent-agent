#include "httpClient.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void HttpClient::get(QUrl url){
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestComplete(QNetworkReply*)));
    networkManager->get(QNetworkRequest(url));
}

void HttpClient::post(QUrl url, QString body){
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestComplete(QNetworkReply*)));
    networkManager->post(QNetworkRequest(url), body.toUtf8());
}

void HttpClient::post(QNetworkRequest request, QString body){
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestComplete(QNetworkReply*)));
    networkManager->post(request, body.toUtf8());
}

void HttpClient::onRequestComplete(QNetworkReply *reply) {
    emit complete(reply);
}
