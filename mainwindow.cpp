#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QSound>
#include <QSize>
#include <QDesktopWidget>
#include <QShortcut>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

/*
 * 1. url address
 * 2. dialog prompt color
 * 3. mainwindow title
 *
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&WebSocketClient::get_instance(), SIGNAL(set_mainwindow_title(QString)),
            this, SLOT(setWindowTitle(QString)));

    if(_login_dlg.exec() != QDialog::Accepted)
    {
        emit exit(0);
    }

    QSize winSize = QDesktopWidget().availableGeometry(this).size();
    resize(winSize * 0.8);

    QPixmap bkgnd(":/images/yukang.jpg");
    _bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    _palette.setBrush(QPalette::Background, _bkgnd);
    this->setPalette(_palette);

    new QShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_T), this, SLOT(http_test()));

    _network_manager = new QNetworkAccessManager;

    _timer = new QTimer();
    connect(_timer, &QTimer::timeout,
            this, [=](){
                    qDebug("heart time out");
                    WebSocketClient::get_instance().send_heart();
    });

    _timer->start(1000*30);
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QPixmap bkgnd(":/images/yukang.jpg");
    _bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    _palette.setBrush(QPalette::Background, _bkgnd);
    this->setPalette(_palette);
}

void MainWindow::http_test()
{
    qDebug("acclerator key");

    QNetworkRequest request(QUrl("http://120.24.61.253:9999/?name=yukang&branch=" + UserAccount::get_instance().get_branch_name_en().toLatin1()));
    _network_manager->get(request);
}

MainWindow::~MainWindow()
{
    delete ui;
}
