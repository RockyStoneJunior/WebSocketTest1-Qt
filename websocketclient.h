#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#pragma execution_character_set("utf-8")

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QAbstractSocket>
#include <QTimer>

#include "mainwindow.h"
#include "httppost.h"

class MainWindow;

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    static WebSocketClient& get_instance(){
        if(_instance == nullptr)
        {
            _instance = create_instance();
        }

        return *_instance;
    }

    void connect_server();
    void send_heart();

protected:
    explicit WebSocketClient(QObject *parent = nullptr);
    virtual ~WebSocketClient(){
        if(_instance != nullptr)
        {
            delete _instance;
            _instance = nullptr;
        }
    }

signals:
    void set_mainwindow_title(QString title);
    void connect_server_result(bool success);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();
    void on_reconnect_result(HttpPostResult result);

private:
    QWebSocket _web_socket;
    HttpPost _http_post;
    QTimer *_timer;

    static WebSocketClient* _instance;
    static WebSocketClient* create_instance(){return new WebSocketClient;}
};

#endif // WEBSOCKETCLIENT_H
