#include <memory>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QtCore/QStandardPaths>
#include <QProcess>

#include <gpgme.h>
#include <gpgme++/data.h>
#include <gpgme++/global.h>
#include <gpgme++/context.h>
#include <gpgme++/engineinfo.h>
#include <gpgme++/keylistresult.h>
#include <gpgme++/importresult.h>
#include <gpgme++/encryptionresult.h>
#include <gpgme++/decryptionresult.h>

#include <qgpgme/importjob.h>
#include <qgpgme/deletejob.h>
#include <qgpgme/decryptjob.h>
#include <qgpgme/encryptjob.h>
#include <qgpgme/protocol.h>
#include <qgpgme/keylistjob.h>
#include <qgpgme/changeownertrustjob.h>


#include "gpg.h"
#include "passkeymodel.h"
#include "passphraseprovider.h"





using namespace GpgME;
using namespace QGpgME;

Gpg::Gpg(QObject* windows)
{
    this->m_passphrase_provider = new UTPassphraseProvider(windows);

    Gpg::initGpgConfig();

    auto error = checkEngine(OpenPGP);
    if (error) {
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
        qFatal("GNUPG Engine check Fail");
    }

    qDebug() << "GNUPG Engine Version is :" << engineInfo(OpenPGP).version();
    qDebug() << "GNUPG Executable is :" << engineInfo(OpenPGP).fileName();
    qDebug() << "GNUPG Home is :" << engineInfo(OpenPGP).homeDirectory();
}

Gpg::~Gpg()
{
    delete this->m_passphrase_provider;
}


QString Gpg::initGpgHome()
{
    QString path = QStandardPaths::writableLocation(
                       QStandardPaths::AppDataLocation).append("/.gpghome");
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return path;
}


QString Gpg::findCommandPath(const QString &command)
{
    // Retrieve the PATH environment variable
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString pathEnv = env.value("PATH");

    // Split the PATH by colon
    QStringList pathDirs = pathEnv.split(":", QString::SkipEmptyParts);

    // Check each directory in the PATH
    foreach (const QString &dir, pathDirs) {
        QFileInfo fileInfo(QDir(dir).filePath(command));

        // If the file exists and is executable, return the path
        if (fileInfo.exists() && fileInfo.isExecutable()) {
            return fileInfo.absoluteFilePath();
        }
    }

    return QString::null;
}

QString Gpg::initGpgExec()
{
    QString path = findCommandPath("gpg");
    if (path.isNull()) {
        qFatal("No valid gpg exec found !");
    }
    return path;
}


void Gpg::initGpgConfig()
{
    initializeLibrary();
    gpgme_set_global_flag("disable-gpgconf", "1");

    QString home = initGpgHome();
    qDebug() << "Gpg home is " << home;
    QString exec = initGpgExec();
    qDebug() << "Gpg exec is " << exec;

    QFile agentConf(home + QStringLiteral("/gpg-agent.conf"));
    agentConf.remove();
    agentConf.open(QIODevice::WriteOnly);
    agentConf.write("allow-loopback-pinentry\n");
    agentConf.close();

    auto err = gpgme_set_engine_info (
                   GPGME_PROTOCOL_OpenPGP,
                   exec.toLocal8Bit().data(),
                   home.toLocal8Bit().data()
               );
    if (err != GPG_ERR_NO_ERROR) {
        qDebug() << "Error code : " << err;
        qDebug() << "Error str : " << gpg_strerror(err);
        qFatal("GPGME set engine info failed !");
    }
}


Error Gpg::decrypt(QByteArray cipher_text)
{
    auto job = openpgp()->decryptJob();

    auto ctx = DecryptJob::context(job);

    ctx->setPassphraseProvider(this->m_passphrase_provider);
    ctx->setPinentryMode(Context::PinentryLoopback);

    QObject::connect(job, &DecryptJob::result,
                     this, &Gpg::decryptResultSlot);

    return job->start(cipher_text);
}

Error Gpg::decryptFromFile(QString path)
{
    qDebug() << "Decrypt from " << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open the File";
        return Error();
    }
    QByteArray cipherText = file.readAll();
    file.close();
    return decrypt(cipherText);
}



void Gpg::decryptResultSlot(const GpgME::DecryptionResult &result, const QByteArray &plainText,
                            const QString &auditLogAsHtml, const GpgME::Error &auditLogError)
{
    if (result.error()) {
        qWarning() << "Something gone wrong on decrypt";
        qDebug() << "Code Error : " << result.error().code();
        qDebug() << "Error str : " << result.error().asString();
    }
    emit decryptResult(result.error(), QString::fromUtf8(plainText));
}


// QPair<Error, QByteArray> Gpg::encrypt(QString str, QString uid, bool ascii_armor, bool text_mode)
// {

//     qDebug() << "Encrypt to QByteArray";
//     auto keys = getKeys(uid);
//     if (keys.first) {
//         return QPair<Error, QByteArray>(keys.first, QByteArray());
//     }

//     auto job = std::unique_ptr<EncryptJob>(openpgp()->encryptJob(ascii_armor, text_mode));

//     QByteArray cipherText;
//     auto result = job->exec(keys.second, str.toUtf8(), Context::AlwaysTrust, cipherText);

//     qDebug() << "Encrypted to QByteArray";
//     return QPair<Error, QByteArray>(result.error(), cipherText);
// }


// Error Gpg::encryptToFile(QString str, QString path, QString uid, bool ascii_armor,
//                          bool text_mode)
// {
//     qDebug() << "Encrypting to file  " << path;
//     QFile file(path);
//     if (!file.open(QIODevice::WriteOnly)) {
//         qWarning() << "Can't open the file to write it" ;
//         return Error();
//     }
//     auto encrypt_ret = encrypt(str, uid, ascii_armor, text_mode);
//     if (encrypt_ret.first) {
//         file.write(encrypt_ret.second);
//     }
//     qDebug() << "Encrypting to file  " << path;
//     return encrypt_ret.first;
// }


Error Gpg::getAllKeys ( bool remote, const bool include_sigs,
                        bool validate )
{
    return getKeys(QString(""), remote, include_sigs, validate);
}

Error Gpg::getKeys(QString pattern_uid, bool remote, bool include_sigs,
                   bool validate)
{
    qDebug() << "Getting the keys " << pattern_uid;
    auto job = openpgp()->keyListJob(remote, include_sigs, validate);

    QObject::connect(job, &KeyListJob::result,
                     this, &Gpg::getKeysJobResultSlot);

    return job->start(QStringList() << pattern_uid, false);
}

void Gpg::getKeysJobResultSlot(const GpgME::KeyListResult &result, const std::vector<GpgME::Key> &keys,
                               const QString &auditLogAsHtml, const GpgME::Error &auditLogError)
{
    if (result.error()) {
        qWarning() << "Something gone wrong on decrypt";
        qDebug() << "Code Error : " << result.error().code();
        qDebug() << "Error str : " << result.error().asString();
    }
    emit getKeysResult(result.error(), keys);
}

Error Gpg::importKeysFromFile(QString path)
{
    qDebug() << "Importing the key file" << path;
    qDebug() << "Decrypt from " << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Can't open the File";
        return Error();
    }
    auto data = file.readAll();
    file.close();

    auto job = openpgp()->importJob();

    QObject::connect(job, &ImportJob::result,
                     this, &Gpg::importKeysFromFileSlot);


    return job->start(data);
}

void Gpg::importKeysFromFileSlot(const GpgME::ImportResult &result, const QString &auditLogAsHtml,
                                 const GpgME::Error &auditLogError)
{
    qDebug() << "numImported" << result.numImported();
    qDebug() << "numSecretKeysImported" << result.numSecretKeysImported();
    qDebug() << "numSecretKeysConsidered" << result.numSecretKeysConsidered();
    qDebug() << "numSecretKeysUnchanged" << result.numSecretKeysUnchanged();
    qDebug() << "numUnchanged" << result.numUnchanged();


    if (result.error()) {
        qWarning() << "Something gone wrong on decrypt";
        qDebug() << "Code Error : " << result.error().code();
        qDebug() << "Error str : " << result.error().asString();
    }
    emit importKeysFromFileResult(result.error());
}

Error Gpg::deleteKey(const Key key)
{
    auto job = openpgp()->deleteJob();

    QObject::connect(job, &DeleteJob::result,
                     this, &Gpg::deleteKeySlot);

    return openpgp()->deleteJob()->start(key, true);
}

void Gpg::deleteKeySlot(const GpgME::Error &error, const QString &auditLogAsHtml, const GpgME::Error &auditLogError)
{

    if (error) {
        qWarning() << "Something gone wrong on decrypt";
        qDebug() << "Code Error : " << error.code();
        qDebug() << "Error str : " << error.asString();
    }
    emit deleteKeyResult(error);
}

