#include <QDebug>
#include <QFile>
#include <QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QStandardPaths>

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

using namespace GpgME;
using namespace QGpgME;

Gpg::Gpg() {
    auto error = initializeLibrary(OpenPGP);
    if (error) {
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
        qFatal("GpgME init fail");
    }

    QString gnuhome = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/gpghome");
    QDir dir(gnuhome);
    if (!dir.exists()) dir.mkpath(".");
    qputenv("GNUPGHOME", gnuhome.toStdString().c_str());
    qDebug() << "GNUPGHOME is :" << qgetenv("GNUPGHOME");

    error = checkEngine(OpenPGP);
    if (error) {
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
        qFatal("Engine check Fail");

    }
    qDebug() << "GpgME Engine Version :" << engineInfo(OpenPGP).version();
}

void Gpg::list_dir() {
    qDebug() << "hello world!";
}

QString Gpg::decrypt(const QByteArray cipherText) {
    /*auto decJob = openpgp()->decryptJob();
    auto ctx = DecryptJob::context(decJob);*/

    /* TODO
     * TestPassphraseProvider provider;
    ctx->setPassphraseProvider(&provider);
    ctx->setPinentryMode(Context::PinentryLoopback);*/

    /*QByteArray plainText;
    decJob->exec(cipherText, plainText);

    return QString::fromUtf8(plainText);*/
}

QString Gpg::decrypt_file(const QString path) {
    /*QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qErrnoWarning("Can't open the File");
        return nullptr;
    }
    QByteArray plainText = file.readAll();
    return this->decrypt(plainText);*/
}

QByteArray Gpg::encrypt(const QString str) {
    /*auto listjob = openpgp()->keyListJob(false, false, false);
    std::vector<Key> keys;
    auto keylistresult = listjob->exec(QStringList() << QStringLiteral("alfa@example.net"), false, keys);

    auto job = openpgp()
            ->encryptJob(
                    true, //ASCII Armor
                    true //Textmode
            );
    QByteArray cipherText;
    auto result = job->exec(keys, QStringLiteral("Hello World").toUtf8(), Context::AlwaysTrust, cipherText);
    return cipherText;*/
}

bool Gpg::encrypt_file(const QString str, const QString path) {
    /*QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qErrnoWarning("Can't open the File");
        return false;
    }
    file.write(Pass::encrypt(str));
    return true;*/
}



QString Gpg::get_key_id(QString uid) {
    qDebug() << "Getting the key id " << uid;
    auto *job = openpgp()->keyListJob(false, false, false);
    std::vector<Key> keys;
    auto result = job->exec(QStringList() << uid, false, keys);
    delete job;

    if (keys.empty()) {
        qDebug() << "No key found for" << uid;
        return nullptr;
    }
    const QString kId = QLatin1String(keys.front().keyID());
    qDebug() << "Id key for " << uid << "is : " <<  kId;
    return kId;
}

QStringList Gpg::get_all_keys_id() {
    qDebug() << "Show all available key";
    auto job = openpgp()->keyListJob(false, false, false);
    std::vector<Key> keys;
    auto result = job->exec(QStringList(""), false, keys);
    delete job;

    if (keys.empty()) {
        qDebug() << "No key found" ;
        return QStringList();
    }

    auto r = QStringList();
    for(const auto &key: keys) {
        r.append(QLatin1String(key.keyID()));
        qDebug() << "Key" << QLatin1String(key.keyID());
    }
    return r;
}

bool Gpg::import_key(QString path) {
    qDebug() << "Importing the key file"  << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open the File";
        return false;
    }

    auto job = openpgp()->importJob();
    auto result = job->exec(file.readAll());

    delete job;
    file.close();

    if(result.error()) {
        qWarning() << "Import go wrong";
        return false;
    }

    qDebug() << "Key imported" << result.numImported();
    qDebug() << "Key not imported" << result.notImported();
    qDebug() << "Key unchanged" << result.numUnchanged();
    qDebug() << "Result null" << result.isNull();
    qDebug() << "newUserIDs" << result.newUserIDs();

    for(const auto &key: result.imports())
        qDebug() << "Key" << key.fingerprint();

    return true;
}