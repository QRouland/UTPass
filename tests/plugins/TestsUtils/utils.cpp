#include <QFile>
#include <QDir>
#include <QUrl>
#include <QUuid>
#include <QtCore/QStandardPaths>
#include <memory>
#include <quazip5/JlCompress.h>

#include "passphraseprovider.h"
#include "utils.h"

TestsUtils::TestsUtils():
m_passphrase_povider(std::unique_ptr<TesTPassphraseProvider>(new TesTPassphraseProvider()))
{}


QString TestsUtils::getTempPath()
{
    // Get the system's temporary directory
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);

    // Generate a unique UUID
    QString uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // Create a new directory using the generated UUID
    QString newTempDir = tempDir + "/" + uuid;

    QDir dir;
    dir.mkpath(newTempDir);

    qDebug() << "TempDir : " << newTempDir;
    return newTempDir;
}


QObject* TestsUtils::getTestPassphraseProvider()
{
    return this->m_passphrase_povider.get();
}


