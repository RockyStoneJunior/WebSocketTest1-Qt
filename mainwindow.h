#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("utf-8")


#include <QMainWindow>
#include <QMediaPlayer>
#include <QPixmap>

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
    void resizeEvent(QResizeEvent *event);
    ~MainWindow();

private slots:
    void http_test();

private:
    Ui::MainWindow *ui;

    LoginDialog _login_dlg;

    QPixmap _bkgnd;
    QPalette _palette;

    QNetworkAccessManager *_network_manager;

    QTimer *_timer;
};

#endif // MAINWINDOW_H
