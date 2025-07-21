#include "mainwindow.h"
#include <QApplication>
#include "loginpage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginpage w;
    w.show();
    return a.exec();
}
