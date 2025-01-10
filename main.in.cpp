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
    qDebug() << "Starting app from main.cpp";
    new QGuiApplication(argc, argv);

    QGuiApplication::setApplicationName("utpass.qrouland");

#ifndef TEST_RUNNER
    auto *view = new QQuickView();
    view->setSource(QUrl(QStringLiteral("qml/Main.qml")));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();
    QObject *mainView = view->findChild<QObject *>("mainView");
    QMetaObject::invokeMethod(
        mainView, "initPass",
        Q_ARG(QVariant, QVariant::fromValue(mainView))
    );
    return QGuiApplication::exec();
#else
    return quick_test_main(argc, argv, "@TESTS_PATH@", "@TESTS_PATH@");
#endif
}
