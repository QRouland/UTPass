#ifndef PASSKEYMODEL_H
#define PASSKEYMODEL_H

#include <QObject>
#include <gpgme++/key.h>

using namespace GpgME;

/**
 * @class UserIdModel
 * @brief A model representing a user ID associated with a GPG key.
 *
 * This class encapsulates the user ID information (UID) for a GPG key, providing access
 * to the UID's identifier, name, and email. It is used as a model for user IDs in the
 * `PassKeyModel` class.
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
    Q_PROPERTY(Key key READ key MEMBER m_key CONSTANT)
    Q_PROPERTY(QString uid READ uid CONSTANT)
    Q_PROPERTY(QList<QObject *> userIds READ userIds CONSTANT)
    Q_PROPERTY(bool isSecret READ isSecret CONSTANT)
    Q_PROPERTY(bool isExpired READ isExpired CONSTANT)

private:
    Key m_key; /**< The GPG key associated with the model. */

public:
    /**
     * @brief Constructs a PassKeyModel for the given GPG key.
     * @param key The GPG key to model.
     */
    PassKeyModel(Key key) : m_key(key) {}

    /**
     * @brief Converts a vector of GPG keys into a list of PassKeyModel objects.
     * @param keys The vector of GPG keys to convert.
     * @return A QList of PassKeyModel objects representing the keys.
     */
    static QList<QObject *> keysToPassKey(std::vector<Key> keys)
    {
        QList<QObject *> ret;
        std::for_each(keys.begin(), keys.end(), [&ret](Key k) {
            ret.append(new PassKeyModel(k));
        });
        return ret;
    };

    /**
     * @brief Gets the GPG key associated with this model.
     * @return The GPG key.
     */
    Key key() const
    {
        return m_key;
    };

    /**
     * @brief Gets the unique identifier (UID) for this GPG key.
     * @return The UID as a QString.
     */
    QString uid() const
    {
        return QString::fromUtf8(m_key.keyID());
    };

    /**
     * @brief Gets the list of user IDs associated with this GPG key.
     * @return A list of UserIdModel objects representing the user IDs.
     */
    QList<QObject *> userIds() const
    {
        auto user_ids = m_key.userIDs();
        QList<QObject *> ret;
        std::for_each(user_ids.begin(), user_ids.end(), [&ret](UserID k) {
            ret.append(new UserIdModel(k));
        });
        return ret;
    };

    /**
     * @brief Checks if the GPG key is a secret key.
     * @return True if the key is a secret key, false otherwise.
     */
    bool isSecret() const
    {
        return m_key.hasSecret();
    };

    /**
     * @brief Checks if the GPG key is expired.
     * @return True if the key is expired, false otherwise.
     */
    bool isExpired() const
    {
        return m_key.isExpired();
    };
};

#endif
