#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QAbstractSocket>
#include <QTimer>

#include "mainwindow.h"

class MainWindow;

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl &url, MainWindow *main_window, QObject *parent = nullptr);

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();

private:
    QWebSocket _web_socket;
    QUrl _url;

    QTimer *_timer;

    MainWindow *_main_window;
};

#endif // WEBSOCKETCLIENT_H
