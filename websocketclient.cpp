﻿#include "websocketclient.h"

WebSocketClient* WebSocketClient::_instance = nullptr;

WebSocketClient::WebSocketClient(QObject *parent):
    QObject(parent)
{

}

void WebSocketClient::connect_server()
{
    connect(&_web_socket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&_web_socket, &QWebSocket::disconnected, this, &WebSocketClient::onClosed);
    connect(&_web_socket, static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),
            this, [=](QAbstractSocket::SocketError error){
                        qDebug(_web_socket.errorString().toLatin1());

                        if(!_timer->isActive())
                        {
                            qDebug("Timer is not active");
                            _timer->start(3000);

                            emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name() + "      连接中...");
                        }
    });

    _timer = new QTimer();
    connect(_timer, &QTimer::timeout,
            this, [=](){
                    qDebug("time out");
                    _web_socket.open(QUrl("ws://localhost:19966/new_order"));
    });

    _web_socket.open(QUrl("ws://localhost:19966/new_order"));
}

void WebSocketClient::onConnected()
{
    qDebug("Connected");
    _timer->stop();

    connect(&_web_socket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    _web_socket.sendTextMessage(UserAccount::get_instance().get_session_id());

    emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name() + "      已连接");
}

void WebSocketClient::onTextMessageReceived(QString message)
{
    qDebug(message.toUtf8());
}

void WebSocketClient::onClosed()
{
    qDebug("Disconnected");
    _timer->start(3000);

    emit set_mainwindow_title("愉康大药房" + UserAccount::get_instance().get_branch_name() + "      连接中...");
}
