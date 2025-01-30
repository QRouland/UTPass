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

    qDebug() << "[TestUtils] TempDir : " << newTempDir;
    return newTempDir;
}

bool TestsUtils::fileExists(QUrl path)
{
    QString p = path.toLocalFile();
    auto ret = QFileInfo::exists(p) && QFileInfo(p).isFile();
    qDebug() << "[TestUtils]" << p << "is existing file :" << ret;
    return ret;
}

void TestsUtils::copyFolder(QUrl sourceFolderUrl, QUrl destFolderUrl)
{
    auto sourceFolder = sourceFolderUrl.toLocalFile();
    auto destFolder = destFolderUrl.toLocalFile();
    QDir sourceDir(sourceFolder);
    if (!sourceDir.exists())
        return;
    QDir destDir(destFolder);
    if (!destDir.exists()) {
        destDir.mkdir(destFolder);
    }
    qDebug() << "[TestUtils]" << "Copy files from" << sourceFolder << "to" << destFolder;
    QStringList files = sourceDir.entryList(QDir::Files);
    for (int i = 0; i < files.count(); i++) {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        QFile::copy(srcName, destName);
        qDebug() << "[TestUtils]" << "Copy file from" << srcName << "to" << destName;
    }
    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < files.count(); i++) {
        QString srcName = sourceFolder + "/" + files[i];
        QString destName = destFolder + "/" + files[i];
        this->copyFolder(srcName, destName);
    }
}

QObject *TestsUtils::getTestPassphraseProvider()
{
    return this->m_passphrase_povider.get();
}


