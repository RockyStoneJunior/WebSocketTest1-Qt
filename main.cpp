#include "mainwindow.h"
#include <QApplication>

#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog dlg;
    if(dlg.exec() != QDialog::Accepted)
    {
        return 0;
    }

    MainWindow w(dlg.getBranch());
    w.show();

    return a.exec();
}
