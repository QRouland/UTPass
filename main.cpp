#include <QGuiApplication>
#include <QCoreApplication>
#include <QUrl>
#include <QString>
#include <QQuickView>
#include <QtQml>


#ifdef TEST_RUNNER
#include <QtQuickTest/quicktest.h>
#endif

int main(int argc, char *argv[])
{
#ifndef TEST_RUNNER
    qDebug() << "Starting app from main.cpp";
    new QGuiApplication(argc, argv);

    QGuiApplication::setApplicationName("utpass.qrouland");
    
    auto *view = new QQuickView();
    view->setSource(QUrl(QStringLiteral("qml/Main.qml")));
    view->show();
    
    return QGuiApplication::exec();
#else
    qDebug() << "Starting tests from main.cpp";
    return quick_test_main(argc, argv, "Tests", "tests/unit");
#endif
    
}
