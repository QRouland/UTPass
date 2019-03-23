#include <QDebug>
#include <QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"
#include "gpg.h"


Pass::Pass()
{
    m_password_store = QStandardPaths::writableLocation(
                           QStandardPaths::AppDataLocation).append("/.password-store");
    QDir dir(m_password_store);
    if (!dir.exists())
        dir.mkpath(".");
    qDebug() << "Password Store is :" << m_password_store;
}

bool Pass::gpgImportKeyFromFile(QString path)
{
    return Gpg::instance()->importKeyFromFile(path);
}

QStringList Pass::gpgListAllKeys()
{
    return Gpg::instance()->getAllKeysId();
}

bool Pass::gpgSetGpghome(QString path)
{
    return Gpg::instance()->setGpghome(path);
}
