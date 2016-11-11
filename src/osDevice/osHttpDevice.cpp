#include <QEventLoop>

#include "osHttpDevice.h"
#include "../httpClient/httpClient.h"

OsHttpDevice::OsHttpDevice(QUrl _url){
    qDebug("OsHttpDevice Constructor");
    name = "HTTP Device";
    httpClient = new HttpClient(this);
    url = _url;
}

void OsHttpDevice::execCommand(QString cmd) {
    qDebug() << "OsHttpDevice::execCommand() - " << name;
    qDebug(cmd.toUtf8());

    //Setup HTTP request slot and initiate proxy call
    connect(httpClient, SIGNAL(complete(QNetworkReply*)), this, SLOT(onHttpComplete(QNetworkReply*)));

    //Build HTTP POST request
    QNetworkRequest request = QNetworkRequest(url);
    request.setRawHeader("Content-Type", "application/json");

    httpClient->post(request, cmd);
    //httpClient->get(url);

    //Wait for signal that proxy call has returned
    QEventLoop loop;
    connect(this, SIGNAL(commandComplete()), &loop, SLOT(quit()));
    loop.exec();
}

void OsHttpDevice::onHttpComplete(QNetworkReply *reply) {
    emit commandComplete();
    emit execCommandComplete(QString(reply->readAll()));
}
