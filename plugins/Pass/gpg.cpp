#include <QDebug>
#include <QFile>
#include <QDir>
#include <QtCore/QStandardPaths>
#include <memory>

#include "gpg.h"
#include "gpgme++/global.h"
#include "gpgme++/context.h"
#include "gpgme++/engineinfo.h"
#include "qgpgme/protocol.h"
#include "qgpgme/keylistjob.h"
#include "gpgme++/keylistresult.h"
#include "qgpgme/importjob.h"
#include "gpgme++/importresult.h"
#include "qgpgme/decryptjob.h"
#include "qgpgme/encryptjob.h"

using namespace GpgME;
using namespace QGpgME;

Gpg::Gpg()
{
    auto error = initializeLibrary(OpenPGP);
    if (error) {
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
        qFatal("GpgME init fail");
    }

    setGpghome();

    error = checkEngine(OpenPGP);
    if (error) {
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
        qFatal("Engine check Fail");
    }
    qDebug() << "GpgME Engine Version :" << engineInfo(OpenPGP).version();
}

bool Gpg::setGpghome(QString path)
{
    QFileInfo file(path);
    if (file.isFile()) {
        qWarning() << "Not a directory GNUPGHOME not change";
        return false;
    }
    if (file.isDir() and !file.isWritable()) {
        qWarning() << "Not a writable directory GNUPGHOME not change";
        return false;
    }
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    m_gpghome = dir;
    qputenv("GNUPGHOME", m_gpghome.absolutePath().toStdString().c_str());
    qDebug() << "GNUPGHOME is :" << qgetenv("GNUPGHOME");
}

QString Gpg::decrypt(const QByteArray cipherText)
{
    /*
    auto decJob = openpgp()->decryptJob();
    auto ctx = DecryptJob::context(decJob);


    TestPassphraseProvider provider;
    ctx->setPassphraseProvider(&provider);
    ctx->setPinentryMode(Context::PinentryLoopback);

    QByteArray plainText;
    decJob->exec(cipherText, plainText);

    return QString::fromUtf8(plainText);*/
}

QString Gpg::decryptFromFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open the File";
        return nullptr;
    }
    QByteArray plainText = file.readAll();
    return decrypt(plainText);
}

QByteArray Gpg::encrypt(QString str, QString uid, bool ascii_armor, bool text_mode)
{
    qDebug() << "Encrypt to QByteArray";

    auto keys = getKeys(uid);

    auto job = openpgp()
               ->encryptJob(
                   ascii_armor,
                   text_mode
               );
    QByteArray cipherText;
    //auto result = job->exec(keys, str.toUtf8(), Context::AlwaysTrust, cipherText);
    return cipherText;
}

bool Gpg::encryptToFile(QString str, QString path, const QString uid, bool ascii_armor,
                        bool text_mode)
{
    qDebug() << "Encrypt to file  " << path;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open the file to write it" ;
        return false;
    }
    file.write(encrypt(str, uid, ascii_armor, text_mode));
    return true;
}

std::vector<Key> Gpg::getKeys(QString pattern_uid, bool remote, bool include_sigs, bool validate)
{
    qDebug() << "Getting the key " << pattern_uid;
    auto *job = openpgp()->keyListJob(remote, include_sigs, validate);

    std::vector<Key> keys;
    auto result = job->exec(QStringList() << pattern_uid, false, keys);
    delete job;

    return keys;
}

QStringList Gpg::getAllKeysId(bool remote, bool include_sigs, bool validate)
{
    qDebug() << "Show all available key";
    auto job = openpgp()->keyListJob(remote, remote, validate);
    std::vector<Key> keys;
    auto result = job->exec(QStringList(""), false, keys);
    delete job;

    if (keys.empty()) {
        qDebug() << "No key found";
        return QStringList();
    }

    auto r = QStringList();
    for (const auto &key : keys) {
        r.append(QLatin1String(key.keyID()));
        qDebug() << "Key" << QLatin1String(key.keyID());
    }
    return r;
}

bool Gpg::importKeyFromFile(QString path)
{
    qDebug() << "Importing the key file" << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open the File";
        return false;
    }

    auto job = openpgp()->importJob();
    auto result = job->exec(file.readAll());

    delete job;
    file.close();

    if (result.error()) {
        qWarning() << "Import go wrong";
        return false;
    }
    return result.imports().size() == 1;
}
