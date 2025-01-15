#ifndef PASS_H
#define PASS_H

#include <QObject>
#include <QUrl>
#include <QVariant>
#include <gpgme++/context.h>

#include "gpg.h"

using namespace GpgME;

/**
 * @class Pass
 * @brief A class for managing password storage using GPG encryption.
 *
 * This class provides functionalities for interacting with password storage, including
 * storing, showing, importing, and deleting passwords securely using GPG encryption.
 * It communicates asynchronously with GPG operations, using signals and slots to propagate results.
 * The class interacts with the `Gpg` class to perform GPG key operations and provides an interface
 * for the user to manage the passwords.
 */
class Pass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password_store READ password_store MEMBER m_password_store CONSTANT)

private slots:
    /**
     * @brief Slot to handle the result of a GPG decryption operation (to show password).
     * @param err The error that occurred during the operation.
     * @param plain_text The decrypted plain text (password).
     */
    void showResult(Error err, QString plain_text);

    /**
     * @brief Slot to handle the result of a GPG key deletion operation.
     * @param err The error that occurred during the operation.
     */
    void deleteGPGKeyResult(Error err);

    /**
     * @brief Slot to handle the result of a GPG key import operation.
     * @param err The error that occurred during the operation.
     */
    void importGPGKeyResult(Error err);

    /**
     * @brief Slot to handle the result of retrieving all GPG keys.
     * @param err The error that occurred during the operation.
     * @param keys_info The list of GPG keys retrieved.
     */
    void getAllGPGKeysResult(Error err, std::vector<GpgME::Key> keys_info);

signals:
    // GPG-related signals
    /**
     * @brief Emitted when a GPG key is successfully deleted.
     */
    void deleteGPGKeySucceed();

    /**
     * @brief Emitted when a GPG key deletion fails.
     * @param message The error message describing the failure.
     */
    void deleteGPGKeyFailed(QString message);

    /**
     * @brief Emitted when a GPG key is successfully imported.
     */
    void importGPGKeySucceed();

    /**
     * @brief Emitted when a GPG key import fails.
     * @param message The error message describing the failure.
     */
    void importGPGKeyFailed(QString message);

    /**
     * @brief Emitted when all GPG keys are successfully retrieved.
     * @param keys_info The list of retrieved keys.
     */
    void getAllGPGKeysSucceed(QVariant keys_info);

    /**
     * @brief Emitted when retrieving GPG keys fails.
     * @param message The error message describing the failure.
     */
    void getAllGPGKeysFailed(QString message);

    // Pass-related signals
    /**
     * @brief Emitted to propagate passphrase dialog response.
     * @param cancel Whether the dialog was cancelled.
     * @param passphrase The passphrase entered, if not cancelled.
     */
    void responsePassphraseDialogPropagate(bool cancel, QString passphrase);

    /**
     * @brief Emitted when a password is successfully retrieved and shown.
     * @param name The name of the password (e.g., service).
     * @param text The password text.
     */
    void showSucceed(QString name, QString text);

    /**
     * @brief Emitted when showing a password fails.
     * @param message The error message describing the failure.
     */
    void showFailed(QString message);

private:
    QString m_password_store; /**< The path to the password store. */
    std::unique_ptr<Gpg> m_gpg; /**< The GPG instance used for encryption/decryption. */
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */
    QString m_show_filename; /**< The filename associated with the password to show. */

public:
    /**
     * @brief Constructs the Pass object and initializes necessary resources.
     */
    Pass();

    /**
     * @brief Destructor for cleaning up resources used by the Pass object.
     */
    ~Pass() override = default;

    /**
     * @brief Gets the path to the password store.
     * @return The path to the password store.
     */
    QString password_store() const
    {
        return m_password_store;
    };

    /**
     * @brief Initializes the Pass object with the given window.
     * @param window The QObject window to interact with.
     */
    Q_INVOKABLE void initialize(QObject *window);

    // GPG-related methods

    /**
     * @brief Deletes the specified GPG key.
     * @param key The GPG key to delete.
     * @return True if the operation was successful, false otherwise.
     */
    Q_INVOKABLE bool deleteGPGKey(Key key);

    /**
     * @brief Imports a GPG key from the given URL.
     * @param url The URL to import the GPG key from.
     * @return True if the operation was successful, false otherwise.
     */
    Q_INVOKABLE bool importGPGKey(QUrl url);

    /**
     * @brief Retrieves all GPG keys.
     * @return True if the operation was successful, false otherwise.
     */
    Q_INVOKABLE bool getAllGPGKeys();

    /**
     * @brief Return the response from the passphrase dialog.
     * @param cancel Whether the dialog was cancelled.
     * @param passphrase The passphrase entered, if not cancelled.
     */
    Q_INVOKABLE void responsePassphraseDialog(bool cancel, QString passphrase);

    // Password store-related methods

    /**
     * @brief Shows the password associated with the specified URL.
     * @param url The URL pointing to the password store entry.
     * @return True if the operation was successful, false otherwise.
     */
    Q_INVOKABLE bool show(QUrl url);
};

#endif
