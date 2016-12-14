#include <QEventLoop>
#include "wflHttpDevice.h"
#include "../httpClient/httpClient.h"

WflHttpDevice::WflHttpDevice(QUrl _url){
    qDebug("OsHttpDevice::OsHttpDevice(QUrl _url)");
    url = _url;
    WflHttpDevice();
}

WflHttpDevice::WflHttpDevice() {
    qDebug("OsHttpDevice::OsHttpDevice()");
    name = "HTTP Device";
    deviceType = "HTTP";
    httpClient = new HttpClient(this);
    connect(httpClient, SIGNAL(complete(QNetworkReply*)), this, SLOT(onHttpComplete(QNetworkReply*)));
}

void WflHttpDevice::execCommand(QByteArray cmd) {
    qDebug() << "OsHttpDevice::execCommand() - " << name;
    qDebug("%s", cmd.data());

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

void WflHttpDevice::setUrl(QUrl _url) {
    url = _url;
}

void WflHttpDevice::onHttpComplete(QNetworkReply *reply) {
    qDebug("OsHttpDevice::onHttpComplete()");
    emit commandComplete();
    emit execCommandComplete(reply->readAll());
}

QByteArray WflHttpDevice::writeRead(QByteArray cmd){
    qDebug("WflHttpDevice::write() NOT IMPLEMENTED !!!!!!!!!");
}

void WflHttpDevice::onWriteReadResponse(QByteArray reply){
    qDebug("WflUartDevice::onWriteReadComplete() NOT IMPLEMENTED !!!!!!!!!");
    emit writeReadComplete();
}

void WflHttpDevice::softReset(){
    qDebug("WflHttpDevice::softReset() NOT IMPLEMENTED !!!!!!!!!");
}


