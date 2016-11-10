#ifndef PROXYCONTROLLER_H
#define PROXYCONTROLLER_H

//QT core includes
#include <QNetworkAccessManager>
#include <QNetworkReply>

//HTTP includes
#include "httprequesthandler.h"
#include "../httpClient/httpClient.h"

//OpenScope device includes
#include "../osDevice/osDevice.h"

class ProxyController : public HttpRequestHandler {
    Q_OBJECT
public:
    ProxyController(QObject* _parent=0, OsDevice* _activeDevice=0);
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void proxyComplete();

private slots:    
    void onComplete(QNetworkReply *reply);

private:
    HttpClient* httpClient;
    QString proxyResponse;
    OsDevice* activeDevice;
};

#endif // PROXYCONTROLLER_H
