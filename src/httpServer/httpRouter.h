#ifndef HTTPROUTER_H
#define HTTPROUTER_H

//HTTP Core Includes
#include "httprequesthandler.h"
#include "staticfilecontroller.h"

//OpenScope device includes
#include "../osDevice/osDevice.h"

class HttpRouter : public HttpRequestHandler {
    Q_OBJECT
public:
    HttpRouter(QObject* _parent=0, OsDevice *_activeDevice=0);
    static StaticFileController* staticFileController;
    void service(HttpRequest& request, HttpResponse& response);

signals:
    void deviceComplete();

private slots:
    void onComplete(QString _reply);


private:       
    OsDevice* activeDevice;
    QString reply;

};

#endif // HTTPROUTER_H
