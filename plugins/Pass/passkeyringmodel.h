#ifndef PASSKEYRINGMODEL_H
#define PASSKEYRINGMODEL_H

#include <QDebug>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSet>

/**
 * @class PassKeyModel
 * @brief A model representing a GPG (GNU Privacy Guard) key.
 *
 */
class PassKeyModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fingerprint MEMBER m_fingerprint CONSTANT)
    Q_PROPERTY(QString keyid MEMBER m_keyid CONSTANT)
    Q_PROPERTY(QVariant userids MEMBER m_userids CONSTANT)
    Q_PROPERTY(bool hasSecret MEMBER m_hasSecret CONSTANT)

private:
    QString m_fingerprint; /**< The fingerprint of the GPG key, used to uniquely identify the key. */
    QString m_keyid; /**< The unique ID associated with the GPG key. */
    QVariant m_userids; /**< A list of user IDs associated with the GPG key. */
    bool m_hasSecret; /**< Indicates whether the GPG key has an associated secret key. */

public:
    /**
     * @brief Constructs a PassKeyModel object using the provided GPG key information.
     *
     * This constructor initializes the PassKeyModel based on a JSON document containing GPG key data.
     * The key data typically includes the key's fingerprint, key ID, associated user IDs, and secret key status.
     *
     * @param key_info A JSON document containing the GPG key data.
     */
    PassKeyModel(QJsonDocument key_info)
    {
        this->m_fingerprint = key_info["fingerprint"].toString();
        qDebug() << "fingerprint : " << this->m_fingerprint;

        this->m_keyid = key_info["keyid"].toString();
        qDebug() << "keyid : " << this->m_keyid;

        auto user_ids_json_array = key_info["userids"].toArray();
        auto userids = QList<QString>();
        for (auto i = user_ids_json_array.begin(), end = user_ids_json_array.end(); i != end; ++i) {
            userids.append((*i).toString());
        }
        this->m_userids = QVariant(userids);
        qDebug() << "userids : " << this->m_userids;

        this->m_hasSecret = key_info["secret key"]["present"].toBool();
        qDebug() << "hasSecret : " << this->m_hasSecret;
    }
};


/**
 * @class PassKeyringModel
 * @brief A model representing a collection of GPG keys.
 *
 * This class serves as a container for multiple GPG keys, typically representing an entire
 * keyring. It provides functionality to manage and retrieve keys, such as fetching all keys
 * in the keyring and determining the length of the keyring.
 *
 * The class also includes logic to distinguish between primary and sub keys, with an option
 * to ignore subkeys if desired.
 */
class PassKeyringModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject *> keys MEMBER m_keys CONSTANT)
    Q_PROPERTY(int length READ length CONSTANT)

private:
    QList<QObject *> m_keys; /**< A list of PassKeyModel objects representing the keys in the keyring. */

public:
    /**
     * @brief Constructs a PassKeyringModel from a list of GPG key JSON documents.
     *
     * This constructor initializes the PassKeyringModel by parsing a list of JSON documents
     * that represent multiple GPG keys. It filters out subkeys and only retains primary keys
     * for inclusion in the keyring.
     *
     * @param key_infos A list of JSON documents representing GPG keys.
     */
    PassKeyringModel(QList<QJsonDocument> key_infos)
    {
        for (auto i = key_infos.begin(), end = key_infos.end(); i != end; ++i) {
            qDebug() << *i;

            // Ignore subkeys and only add primary keys to the model.
            if ((*i)["primary key grip"].isUndefined()) {
                this->m_keys.append(new PassKeyModel(*i));
            } else {
                qDebug() << "Subkey info " << (*i)["keyid"].toString() << "ignored";
            }
        }
    }

    /**
     * @brief Destructor for PassKeyringModel.
     *
     * Cleans up dynamically allocated PassKeyModel objects within the keyring.
     */
    ~PassKeyringModel()
    {
        qDeleteAll(this->m_keys);
    }

    /**
     * @brief Retrieves the number of keys in the keyring.
     *
     * This function returns the number of primary keys present in the keyring.
     *
     * @return The number of keys in the keyring.
     */
    int length()
    {
        return this->m_keys.length();
    }
};

#endif // PASSKEYRINGMODEL_H
