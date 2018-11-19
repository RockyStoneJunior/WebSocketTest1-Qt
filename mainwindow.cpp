#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString *branch, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _branch_name = branch;

    setWindowTitle("愉康大药房" + *branch);

    _web_socket_client = new WebSocketClient(QUrl("ws://localhost:19966/new_order"), this, nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}
