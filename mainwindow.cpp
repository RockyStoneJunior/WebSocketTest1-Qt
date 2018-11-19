#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("愉康大药房" + UserAccount::get_instance().get_branch_name());

    connect(&WebSocketClient::get_instance(), SIGNAL(set_mainwindow_title(QString)),
            this, SLOT(setWindowTitle(QString)));

    if(_login_dlg.exec() != QDialog::Accepted)
    {
        emit exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
