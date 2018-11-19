#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")


#include <QMainWindow>
#include "websocketclient.h"

namespace Ui {
class MainWindow;
}

class WebSocketClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString *branch, QWidget *parent = 0);
    ~MainWindow();

    QString &get_branch_name(){ return *_branch_name;}

private:
    Ui::MainWindow *ui;

    QString *_branch_name;

    WebSocketClient *_web_socket_client;
};

#endif // MAINWINDOW_H
