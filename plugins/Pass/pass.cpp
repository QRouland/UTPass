#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"
#include "gpg.h"
#include "passphraseprovider.h"
#include "passkeymodel.h"



Pass::Pass(): m_password_store (QStandardPaths::writableLocation(
                                        QStandardPaths::AppDataLocation).append("/.password-store"))
{}

void Pass::init(QObject *window)
{
    if (!window) {
        qFatal("window is invalid. Abording.");
    }
    Gpg::instance()->setWindow(window);

    QDir dir(m_password_store);
    if (!dir.exists())
        dir.mkpath(".");
    qDebug() << "Password Store is :" << m_password_store;
}

void Pass::decrypt(QUrl url)
{
    qDebug() << "Start decrypting";
    auto decrypt_ret = Gpg::instance()->decryptFromFile(url.toLocalFile());
    if (decrypt_ret.first) {
        qDebug() << "Decrypt Failed";
        emit decryptFailed();
    } else if (decrypt_ret.second.isNull()) {
        qDebug() << "Decrypt Canceled";
        emit decryptCanceled();
    } else {
        qDebug() << "Decrypt OK";
        emit decrypted(decrypt_ret.second);
    }
}

bool Pass::gpgDeleteKeyId(QString id)
{
    qDebug() << "Start deleting Key id " << id;
    return !Gpg::instance()->deleteKeyId(id);
}

bool Pass::gpgImportKeyFromFile(QUrl url)
{
    qDebug() << "Start importing Key from " << url;
    return !Gpg::instance()->importKeysFromFile(url.toLocalFile());
}

QVariant Pass::gpgGetAllKeysModel()
{
    return QVariant::fromValue(PassKeyModel::keysToPassKeyQObjectList(
                                   Gpg::instance()->getAllKeys().second));
}

QString Pass::getPasswordStore()
{
    return m_password_store;
}
