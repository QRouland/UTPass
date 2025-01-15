#ifndef PASSKEYMODEL_H
#define PASSKEYMODEL_H

#include <QObject>
#include <gpgme++/key.h>

using namespace GpgME;

class UserIdModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ uid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString email READ email CONSTANT)

private:
    UserID m_user_id;
public:
    UserIdModel(UserID key):
        m_user_id(key)
    {};

    QString uid() const
    {
        return QString::fromUtf8(m_user_id.id());
    };
    QString name() const
    {
        return QString::fromUtf8(m_user_id.name());
    };
    QString email() const
    {
        return QString::fromUtf8(m_user_id.email());
    };
};

class PassKeyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Key key READ key MEMBER m_key CONSTANT)
    Q_PROPERTY(QString uid READ uid CONSTANT)
    Q_PROPERTY(QList<QObject *> userIds READ userIds CONSTANT)
    Q_PROPERTY(bool isSecret READ isSecret CONSTANT)
    Q_PROPERTY(bool isExpired READ isExpired CONSTANT)

    Key m_key;
public:
    PassKeyModel(Key key):
        m_key(key)
    {};

    static QList<QObject *> keysToPassKey(std::vector<Key> keys)
    {
        QList<QObject *> ret;
        std::for_each(keys.begin(), keys.end(), [&ret](Key k) {
            ret.append(new PassKeyModel(k));
        });
        return ret;
    };

    Key key() const
    {
        return m_key;
    };

    QString uid() const
    {
        return QString::fromUtf8(m_key.keyID());
    };

    QList<QObject *> userIds() const
    {
        auto user_ids = m_key.userIDs();
        QList<QObject *> ret;
        std::for_each(user_ids.begin(), user_ids.end(), [&ret](UserID k) {
            ret.append(new UserIdModel(k));
        });
        return ret;
    };

    bool isSecret() const
    {
        return m_key.hasSecret();
    };

    bool isExpired() const
    {
        return m_key.hasSecret();
    };
};



#endif
