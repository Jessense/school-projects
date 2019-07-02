#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
