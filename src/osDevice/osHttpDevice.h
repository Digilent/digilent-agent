#ifndef OSHTTPDEVICE_H
#define OSHTTPDEVICE_H

//QT core includes
#include <QUrl>

#include "osDevice.h"
#include "../httpClient/httpClient.h"

class OsHttpDevice : public OsDevice {
    Q_OBJECT

public:
    OsHttpDevice();
    OsHttpDevice(QUrl _url);

    virtual void execCommand(QByteArray cmd);
    void setUrl(QUrl _url);

    QUrl url;

signals:
    void commandComplete();

public slots:
    void onHttpComplete(QNetworkReply *reply);

private:
    HttpClient* httpClient;
};

#endif // OSHTTPDEVICE_H
