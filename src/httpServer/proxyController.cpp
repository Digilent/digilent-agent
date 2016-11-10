//QT core includes
#include <QtCore>

#include "proxyController.h"

ProxyController::ProxyController(QObject* parent, QString *activeDevice) : HttpRequestHandler(parent) {
    httpClient = new HttpClient(this);
    m_activeDevice = activeDevice;
}

void ProxyController::service(HttpRequest &request, HttpResponse &response) {

    qDebug() << "Request Path: " << request.getPath();
    //Setup HTTP request slot and initiate proxy call
    connect(httpClient, SIGNAL(complete(QNetworkReply*)), this, SLOT(onComplete(QNetworkReply*)));
    httpClient->get(QString("http://www.feeds.labviewmakerhub.com/"));

    //Wait for signal that proxy call has returned
    QEventLoop loop;
    connect(this, SIGNAL(proxyComplete()), &loop, SLOT(quit()));
    loop.exec();

    //Return proxy call response to original requester
    //response.write(proxyResponse.toUtf8());
    response.write(m_activeDevice->toUtf8());
}

void ProxyController::onComplete(QNetworkReply *reply) {
    proxyResponse = QString(reply->readAll());
    emit proxyComplete();
}

