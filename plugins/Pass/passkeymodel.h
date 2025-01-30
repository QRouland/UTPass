#ifndef PASSKEYMODEL_H
#define PASSKEYMODEL_H

#include <QDebug>
#include <QObject>
#include <QSet>
#include <gpgme++/key.h>

using namespace GpgME;

/**
 * @class UserIdModel
 * @brief A model representing a user ID associated with a GPG key.
 *
 * This class encapsulates the user ID information (UID) for a GPG key, providing access
 * to the UID's identifier, name, and email.
 */
class UserIdModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ uid CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString email READ email CONSTANT)

private:
    UserID m_user_id; /**< The GPG UserID associated with the model. */

public:
    /**
     * @brief Constructs a UserIdModel for the given UserID.
     * @param key The GPG UserID to model.
     */
    UserIdModel(UserID key) : m_user_id(key) {}

    /**
     * @brief Gets the unique identifier (UID) for this user ID.
     * @return The UID as a QString.
     */
    QString uid() const
    {
        return QString::fromUtf8(m_user_id.id());
    };

    /**
     * @brief Gets the name associated with this user ID.
     * @return The name as a QString.
     */
    QString name() const
    {
        return QString::fromUtf8(m_user_id.name());
    };

    /**
     * @brief Gets the email associated with this user ID.
     * @return The email as a QString.
     */
    QString email() const
    {
        return QString::fromUtf8(m_user_id.email());
    };
};

/**
 * @class PassKeyModel
 * @brief A model representing a GPG key.
 *
 * This class encapsulates the properties of a GPG key, including its key ID, associated
 * user IDs, secret key status, and expiration status. It is used as a model for managing
 * GPG keys within an application, providing access to the key's data and its associated user IDs.
 */
class PassKeyModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fingeprint READ fingeprint MEMBER m_fingeprint CONSTANT)
    // Q_PROPERTY(QString uid READ uid CONSTANT)
    // Q_PROPERTY(QList<QObject *> userIds READ userIds CONSTANT)
    // Q_PROPERTY(bool isSecret READ isSecret CONSTANT)
    // Q_PROPERTY(bool isExpired READ isExpired CONSTANT)

private:
    QString m_fingeprint; /**< The key fingeprint. */

public:
    /**
     * @brief Constructs a PassKeyModel for the given GPG key.
     * @param key The GPG key to model.
     */
    PassKeyModel(QString fingeprint) : m_fingeprint(fingeprint) {}


    QString fingeprint() const
    {
        return m_fingeprint;
    };

    // /**
    //  * @brief Gets the GPG key associated with this model.
    //  * @return The GPG key.
    //  */
    // Key key() const
    // {
    //     return m_key;
    // };

    // /**
    //  * @brief Gets the unique identifier (UID) for this GPG key.
    //  * @return The UID as a QString.
    //  */
    // QString uid() const
    // {
    //     return QString::fromUtf8(m_key.keyID());
    // };

    // /**
    //  * @brief Gets the list of user IDs associated with this GPG key.
    //  * @return A list of UserIdModel objects representing the user IDs.
    //  */
    // QList<QObject *> userIds() const
    // {
    //     auto user_ids = m_key.userIDs();
    //     QList<QObject *> ret;
    //     std::for_each(user_ids.begin(), user_ids.end(), [&ret](UserID k) {
    //         ret.append(new UserIdModel(k));
    //     });
    //     return ret;
    // };

    // /**
    //  * @brief Checks if the GPG key is a secret key.
    //  * @return True if the key is a secret key, false otherwise.
    //  */
    // bool isSecret() const
    // {
    //     return m_key.hasSecret();
    // };

    // /**
    //  * @brief Checks if the GPG key is expired.
    //  * @return True if the key is expired, false otherwise.
    //  */
    // bool isExpired() const
    // {
    //     return m_key.isExpired();
    // };
};


/**
 * @class PassKeyModel
 * @brief A model representing a GPG key.
 *
 * This class encapsulates the properties of a GPG key, including its key ID, associated
 * user IDs, secret key status, and expiration status. It is used as a model for managing
 * GPG keys within an application, providing access to the key's data and its associated user IDs.
 */
class PassKeyringModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int length READ length CONSTANT)

private:
    QList<PassKeyModel*> m_keys;

public:
    /**
     * @brief Constructs a PassKeyModel for the given GPG key.
     * @param key The GPG key to model.
     */
    PassKeyringModel(QSet<QString> fingeprints)
    {
        QSet<QString>::iterator i;
        for (auto i = fingeprints.begin(), end = fingeprints.end(); i != end; ++i) {
            this->m_keys.append(new PassKeyModel(*i));
        }
        qDebug() << "Test : " << this->m_keys;
    }

    ~PassKeyringModel() {
        qDeleteAll(this->m_keys);
    }

    int length() {
        qDebug() << "Test2 : " << this->m_keys.length();
        return this->m_keys.length();
    }
};
#endif
