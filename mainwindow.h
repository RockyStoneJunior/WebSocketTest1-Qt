#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")


#include <QMainWindow>
#include "websocketclient.h"
#include "useraccount.h"
#include "logindialog.h"

namespace Ui {
class MainWindow;
}

class WebSocketClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    LoginDialog _login_dlg;
};

#endif // MAINWINDOW_H
