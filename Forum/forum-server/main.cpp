#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include "user.h"
#include "usermodel.h"
#include "commentmodel.h"
#include "postmodel.h"
#include "backend.h"
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    Backend backend;
//    qmlRegisterType<Backend>("io.qt.Backend", 1, 0, "Backend");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", &backend);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;




    return app.exec();
}
