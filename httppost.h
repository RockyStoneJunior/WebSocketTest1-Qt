#ifndef HTTPPOST_H
#define HTTPPOST_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

class HttpPost : public QObject
{
    Q_OBJECT
public:
    explicit HttpPost(QObject *parent = nullptr);

    void send_login_post();
    void send_register_post();

signals:

public slots:

private:
    QNetworkAccessManager *_network_manager;
    QNetworkRequest _network_request;
};

#endif // HTTPPOST_H
