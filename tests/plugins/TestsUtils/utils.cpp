#include <QFile>
#include <QDir>
#include <QUrl>
#include <QUuid>
#include <QtCore/QStandardPaths>
#include <quazip5/JlCompress.h>

#include "utils.h"


QString TestsUtils::getTempPath() {
    qFatal("yp");
    // Get the system's temporary directory
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    qDebug() << "TempDir : " << tempDir;

    // Generate a unique UUID
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // Create a new directory using the generated UUID
    QString newTempDir = tempDir + "/" + uuid;

    QDir dir;
    if (!dir.exists(newTempDir)) {
        // Create the directory
        if (dir.mkpath(newTempDir)) {
            return newTempDir;  // Return the path if successful
        } else {
            return "Failed to create directory";  // Return an error message
        }
    } else {
        return newTempDir;  // If the directory already exists, return its path
    }
}



