#include <QEventLoop>
#include "httpClient.h"

HttpClient::HttpClient(QObject *parent) : QThread(parent), quit(false) {

}

void HttpClient::post(QNetworkRequest request, QString body) {
    QMutexLocker locker(&mutex);
    this->request = request;
    this->body = body;
    this->method = "post";
    if( !isRunning()) {
        start();
    } else {
        cond.wakeOne();
    }
}

void HttpClient::get(QUrl url){
    QMutexLocker locker(&mutex);
    this->request = request;
    this->url = url;
    this->method = "get";
    if( !isRunning()) {
        start();
    } else {
        cond.wakeOne();
    }
}

HttpClient::~HttpClient() {
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
    qDebug("HttpClientThread Distructor");
}

void HttpClient::run() {
    //Protect against race condition when thread is called multiple times
    mutex.lock();
    QNetworkRequest _request = request;
    QString _body = body;
    QString _method = method;
    QUrl _url = url;
    mutex.unlock();

    QNetworkAccessManager networkManager;
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRequestComplete(QNetworkReply*)));
    if(_method == "get") {

        networkManager.get(QNetworkRequest(_url));
    } else if(_method == "post") {
        networkManager.post(_request, _body.toUtf8());
    }

    //Wait for signal that proxy call has returned
    QEventLoop loop;
    connect(this, SIGNAL(reqestComplete()), &loop, SLOT(quit()));
    loop.exec();
    qDebug("HttpClient Done");
    return;
}

void HttpClient::onRequestComplete(QNetworkReply *reply) {
    qDebug("HttpClient::onRequestComplete()");
    emit complete(reply);
    emit reqestComplete();
}
