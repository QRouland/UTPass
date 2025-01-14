#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"
#include "gpg.h"
#include "passkeymodel.h"



Pass::Pass(): m_password_store (QStandardPaths::writableLocation(
                                        QStandardPaths::AppDataLocation).append("/.password-store"))
{}

void Pass::initialize(QObject *window)
{
    if (!window) {
        qFatal("window is invalid. Abording.");
    }

    Gpg::instance()->setWindow(window);

    QDir dir(m_password_store);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    qInfo() << "Password Store is :" << m_password_store;
}

void Pass::show(QUrl url)
{
    qInfo() << "Decrypting";
    auto decrypt_ret = Gpg::instance()->decryptFromFile(url.toLocalFile());
    if (decrypt_ret.first) {
        qInfo() << "Decrypt Failed";
        emit decryptFailed();
    } else if (decrypt_ret.second.isNull()) {
        qInfo() << "Decrypt Canceled";
        emit decryptCanceled();
    } else {
        qInfo() << "Decrypt OK";
        emit decrypted(decrypt_ret.second);
    }
}

bool Pass::deleteGPGKey(QString id)
{
    qInfo() << "Deleting Key id " << id;
    return !Gpg::instance()->deleteKeyId(id);
}

bool Pass::importGPGKey(QUrl url)
{
    qInfo() << "Importing Key from " << url;
    return !Gpg::instance()->importKeysFromFile(url.toLocalFile());
}

QVariant Pass::getAllGPGKeys()
{
    qInfo() << "Getting all key form gpg ";
    return QVariant::fromValue(PassKeyModel::keysToPassKey(
                                   Gpg::instance()->getAllKeys().second)); // TODO Error handling
}

