#include <QGuiApplication>
#include <QCoreApplication>
#include <QUrl>
#include <QString>
#include <QQuickView>
#include "gpgme++/global.h"

int main(int argc, char *argv[])
{
    new QGuiApplication(argc, argv);
    QGuiApplication::setApplicationName("utpass.qrouland");

    qDebug() << "Starting app from main.cpp";

    auto *view = new QQuickView();
    view->setSource(QUrl(QStringLiteral("qml/Main.qml")));
    view->show();

    return QGuiApplication::exec();
}
