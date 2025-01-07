#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"
#include "git.h"
#include "gpg.h"
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
    qInfo() << "Password Store is :" << m_password_store;
}

void Pass::decrypt(QUrl url)
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

bool Pass::gpgDeleteKeyId(QString id)
{
    qInfo() << "Deleting Key id " << id;
    return !Gpg::instance()->deleteKeyId(id);
}

bool Pass::gpgImportKeyFromFile(QUrl url)
{
    qInfo() << "Importing Key from " << url;
    return !Gpg::instance()->importKeysFromFile(url.toLocalFile());
}

QVariant Pass::gpgGetAllKeysModel()
{
    qInfo() << "Getting all key form gpg ";
    return QVariant::fromValue(PassKeyModel::keysToPassKeyQObjectList(
                                   Gpg::instance()->getAllKeys().second));
}

QString Pass::getPasswordStore()
{
    return m_password_store;
}

bool Pass::gitClone(QString url)
{
    qInfo() << "Cloning . password_store from " << url;
    return Git::instance()->clone(url, m_password_store);
}
