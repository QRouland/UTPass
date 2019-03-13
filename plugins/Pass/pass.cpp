#include <QDebug>
#include <QDir>
#include <QString>
#include "pass.h"

Pass::Pass() {

}

void Pass::cmd_show(const QString path) {
    QDir dir = QDir(path);
    if (!dir.exists()) {
        qWarning("Path to show not found");
        return;
    }
}

void Pass::cmd_version() {

}