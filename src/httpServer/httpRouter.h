#ifndef HTTPROUTER_H
#define HTTPROUTER_H

//QT core includes
#include <QtCore>

//HTTP Core Includes
#include "httprequesthandler.h"
#include "staticfilecontroller.h"

//OpenScope device includes
#include "../osDevice/osDevice.h"

class HttpRouter : public HttpRequestHandler {
    Q_OBJECT
public:
    HttpRouter(QObject* _parent=0, OsDevice **_activeDevice=0);
    static StaticFileController* staticFileController;
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void deviceComplete();

private slots:
    void onComplete(QString reply);


private:       
    OsDevice** activeDevice;
    QEventLoop loop;
    QString reply;
    bool waitingForResponse;
};

#endif // HTTPROUTER_H
