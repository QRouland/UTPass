#ifndef GPG_H
#define GPG_H

#include "passkeymodel.h"
#include "passphraseprovider.h"
#include <memory>
#include <QQuickWindow>
#include <gpgme++/context.h>
#include <qgpgme/changeownertrustjob.h>
#include <QSemaphore>

#include <gpgme.h>
#include <qgpgme/importjob.h>
#include <qgpgme/deletejob.h>
#include <qgpgme/decryptjob.h>
#include <qgpgme/encryptjob.h>
#include <qgpgme/protocol.h>
#include <qgpgme/keylistjob.h>
#include <qgpgme/changeownertrustjob.h>


using namespace GpgME;
using namespace QGpgME;

/**
 * @class Gpg
 * @brief A class for managing GPG key operations such as key import, decryption, and deletion.
 *
 * This class integrates with the GPGME (GnuPG Made Easy) library to provide functionalities
 * for interacting with GPG keys, including decrypting messages, importing keys from files,
 * listing keys, and deleting keys.
 */
class Gpg: public QObject
{
    Q_OBJECT
    Q_PROPERTY(UTPassphraseProvider* passphrase_provider READ passphrase_provider MEMBER  m_passphrase_provider )

private slots:
    /**
     * @brief Slot to handle the result of a decryption operation.
     * @param result The result of the decryption operation.
     * @param plain_text The decrypted text.
     * @param auditLogAsHtml The HTML formatted audit log for the operation.
     * @param auditLogError The error associated with the audit log, if any.
     */
    void decryptResultSlot(
        const DecryptionResult &result,
        const QByteArray &plain_text,
        const QString &auditLogAsHtml,
        const Error &auditLogError
    );

    /**
     * @brief Slot to handle the result of a key retrieval operation.
     * @param result The result of the key retrieval operation.
     * @param keys A vector of keys retrieved.
     * @param auditLogAsHtml The HTML formatted audit log for the operation, if any.
     * @param auditLogError The error associated with the audit log, if any.
     */
    void getKeysJobResultSlot(
        const GpgME::KeyListResult &result,
        const std::vector<GpgME::Key> &keys,
        const QString &auditLogAsHtml,
        const GpgME::Error &auditLogError
    );

    /**
     * @brief Slot to handle the result of a key import operation.
     * @param result The result of the import operation.
     * @param auditLogAsHtml The HTML formatted audit log for the operation, if any.
     * @param auditLogError The error associated with the audit log, if any.
     */
    void importKeysFromFileSlot(
        const GpgME::ImportResult &result,
        const QString &auditLogAsHtml,
        const GpgME::Error &auditLogError
    );

    /**
     * @brief Slot to handle the result of a key deletion operation.
     * @param result The error result of the deletion operation.
     * @param auditLogAsHtml The HTML formatted audit log for the operation, if any.
     * @param auditLogError The error associated with the audit log, if any.
     */
    void deleteKeySlot(
        const GpgME::Error &result,
        const QString &auditLogAsHtml,
        const GpgME::Error &auditLogError
    );

signals:
    /**
     * @brief Signal emitted when keys are imported from a file.
     * @param err The error that occurred during the import operation, if any.
     */
    void importKeysFromFileResult(Error err);

    /**
     * @brief Signal emitted when keys are retrieved.
     * @param err The error that occurred during the retrieval operation, if any.
     * @param keys A vector of keys retrieved.
     */
    void getKeysResult(Error err, std::vector<GpgME::Key> keys);

    /**
     * @brief Signal emitted when a key is deleted.
     * @param err The error that occurred during the deletion operation, if any.
     */
    void deleteKeyResult(Error err);

    /**
     * @brief Signal emitted when a decryption operation completes.
     * @param err The error that occurred during decryption, if any.
     * @param plain_text The decrypted message.
     */
    void decryptResult(Error err, QString plain_text);

private:
    UTPassphraseProvider *m_passphrase_provider; /**< The passphrase provider used for authentication. */

    /**
     * @brief Finds the path of a command in the system's environment.
     * @param command The name of the command to find.
     * @return The full path to the command.
     */
    QString findCommandPath(const QString &command);

    /**
     * @brief Initializes the GPG home directory.
     * @return The path to the GPG home directory.
     */
    QString initGpgHome();

    /**
     * @brief Initializes the GPG executable path.
     * @return The path to the GPG executable.
     */
    QString initGpgExec();

    /**
     * @brief Initializes the GPG configuration.
     */
    void initGpgConfig();

    /**
     * @brief Retrieves a GPG key by UID.
     * @param uid The UID of the key to retrieve.
     * @param remote Whether to fetch the key from a remote keyserver (default: false).
     * @param include_sigs Whether to include signatures (default: false).
     * @param validate Whether to validate the key (default: false).
     * @return The error result of the operation.
     */
    Error getKey(QString uid, bool remote = false, bool include_sigs = false, bool validate = false);

public:
    /**
     * @brief Constructs a Gpg object and initializes necessary resources.
     * @param window The window object for interacting with the user interface.
     */
    Gpg(QObject* window);

    /**
     * @brief Destroys the Gpg object and cleans up resources.
     */
    ~Gpg();

    /**
     * @brief Gets the passphrase provider used for GPG authentication.
     * @return The passphrase provider.
     */
    UTPassphraseProvider *passphrase_provider() const
    {
        return m_passphrase_provider;
    }


    /**
     * @brief Imports GPG keys from a file.
     * @param path The path to the file containing the keys.
     * @return The error result of the import operation.
     */
    Error importKeysFromFile(const QString path);

    /**
     * @brief Retrieves keys matching the provided UID pattern.
     * @param pattern_uid The UID pattern to search for.
     * @param remote Whether to fetch the key from a remote keyserver (default: false).
     * @param include_sigs Whether to include signatures (default: false).
     * @param validate Whether to validate the key (default: false).
     * @return The error result of the operation.
     */
    Error getKeys(const QString pattern_uid, bool remote = false, bool include_sigs = false, bool validate = false);

    /**
     * @brief Retrieves all keys from the GPG keyring.
     * @param remote Whether to fetch the keys from a remote keyserver (default: false).
     * @param include_sigs Whether to include signatures (default: false).
     * @param validate Whether to validate the keys (default: false).
     * @return The error result of the operation.
     */
    Error getAllKeys(bool remote = false, bool include_sigs = false, bool validate = false);

    /**
     * @brief Deletes a specified GPG key.
     * @param key The key to delete.
     * @return The error result of the deletion operation.
     */
    Error deleteKey(const Key key);

    /**
     * @brief Decrypts a given ciphertext.
     * @param cipher_text The ciphertext to decrypt.
     * @return The error result of the decryption operation.
     */
    Error decrypt(const QByteArray cipher_text);

    /**
     * @brief Decrypts the contents of a file.
     * @param path The path to the file to decrypt.
     * @return The error result of the decryption operation.
     */
    Error decryptFromFile(const QString path);

    // Error encrypt (QString str,  QString uid,  bool ascii_armor = true, bool text_mode = true);
};

#endif
