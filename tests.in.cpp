#include <QGuiApplication>
#include <QCoreApplication>
#include <QUrl>
#include <QString>
#include <QQuickView>
#include <QtQml>


#include <QtQuickTest/quicktest.h>


int main(int argc, char *argv[])
{
    qDebug() << "Starting app from tests.cpp";
    new QGuiApplication(argc, argv);

    QGuiApplication::setApplicationName("utpass.qrouland");

    return quick_test_main(argc, argv, "@TESTS_PATH@", "@TESTS_PATH@");
}
