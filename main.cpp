#include "mainwindow.h"
#include "srvrclntcomm.h"

#include <QApplication>
#include <QTimer>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
