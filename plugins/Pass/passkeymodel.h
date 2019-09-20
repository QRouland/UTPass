#ifndef PASSKEYMODEL_H
#define PASSKEYMODEL_H

#include <QObject>
#include <gpgme++/key.h>

using namespace GpgME;

class PassKeyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ uid WRITE setUid NOTIFY uidChanged MEMBER m_uid)
    Q_PROPERTY(bool secret READ secret WRITE setSecret NOTIFY secretChanged MEMBER m_secret)
    Q_PROPERTY(bool expired READ expired WRITE setExpired NOTIFY expiredChanged MEMBER m_expired)

    QString m_uid;
    bool m_secret;
    bool m_expired;

public:
    PassKeyModel(QString uid, bool secret, bool expired):
        m_uid(uid),
        m_secret(secret),
        m_expired(expired)
    {};

    PassKeyModel(Key key):
        PassKeyModel(QString::fromUtf8(key.keyID()), key.hasSecret(), key.isExpired())
    {};

    static QList<QObject *> keysToPassKeyQObjectList(std::vector<Key> keys)
    {
        QList<QObject *> r;
        std::for_each(keys.begin(), keys.end(), [&r](Key k) {
            r.append(new PassKeyModel(k));
        });
        return r;
    };

    QString uid() const
    {
        return m_uid;
    };
    bool secret() const
    {
        return m_secret;
    };
    bool expired() const
    {
        return m_expired;
    };

    void setUid(QString uid)
    {
        m_uid = uid;
        emit uidChanged(uid);
    }
    void setSecret(bool secret)
    {
        m_secret = secret;
        emit secretChanged(secret);
    }
    void setExpired(bool expired)
    {
        m_expired = expired;
        emit expiredChanged(expired);
    }

signals:
    void uidChanged(QString);
    void secretChanged(bool);
    void expiredChanged(bool);
};
#endif
