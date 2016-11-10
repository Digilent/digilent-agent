#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

//QT core includes
#include <QNetworkAccessManager>
#include <QNetworkReply>

//HTTP includes
#include "httprequesthandler.h"
#include "../httpClient/httpclient.h"

class ProxyController : public HttpRequestHandler {
    Q_OBJECT
public:
    ProxyController(QObject* parent=0);
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void proxyComplete();

private slots:    
    void onComplete(QNetworkReply *reply);

private:
    HttpClient* httpClient;
    QString proxyResponse;
};

#endif // PROXYCONTROLLER_H
