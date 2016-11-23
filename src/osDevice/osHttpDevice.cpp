#include <QEventLoop>
#include "osHttpDevice.h"
#include "../httpClient/httpClient.h"

OsHttpDevice::OsHttpDevice(QUrl _url){
    qDebug("OsHttpDevice::OsHttpDevice(QUrl _url)");
    url = _url;
    OsHttpDevice();
}

OsHttpDevice::OsHttpDevice() {
    qDebug("OsHttpDevice::OsHttpDevice()");
    name = "HTTP Device";
    deviceType = "HTTP";
    httpClient = new HttpClient(this);
    connect(httpClient, SIGNAL(complete(QNetworkReply*)), this, SLOT(onHttpComplete(QNetworkReply*)));
}

void OsHttpDevice::execCommand(QByteArray cmd) {
    qDebug() << "OsHttpDevice::execCommand() - " << name;
    qDebug(cmd);

    //Build HTTP POST request
    QNetworkRequest request = QNetworkRequest(url);
    request.setRawHeader("Content-Type", "application/json");

    QEventLoop loop;
    connect(this, SIGNAL(commandComplete()), &loop, SLOT(quit()));
    httpClient->post(request, cmd);

    //Wait for signal that proxy call has returned


    loop.exec();
    qDebug("OsHttpDevice Done");
}

void OsHttpDevice::setUrl(QUrl _url) {
    url = _url;
}

void OsHttpDevice::onHttpComplete(QNetworkReply *reply) {
    qDebug("OsHttpDevice::onHttpComplete()");
    emit commandComplete();
    emit execCommandComplete(reply->readAll());
}
