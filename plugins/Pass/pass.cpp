#include <QDebug>
#include <QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"

Pass::Pass(){
    pass_store = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation).append("/.password-store");
    QDir dir(pass_store);
    if (!dir.exists())
        dir.mkpath(".");
    qDebug() << "Password Store is :" << pass_store;
}

void Pass::speak()
{
    qDebug() << "Starting app from main.cpp";
}
