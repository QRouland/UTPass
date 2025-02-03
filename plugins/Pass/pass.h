#ifndef PASS_H
#define PASS_H


#include <QDebug>
#include <QObject>
#include <QUrl>
#include <QVariant>
#include <QSemaphore>
#include <memory>
extern "C" {
#include <rnp/rnp.h>
}
#include "passkeyringmodel.h"
/**
 * @class Pass
 * @brief A class for managing password storage using GPG encryption.
 *
 * This class provides functionalities for interacting with password storage, including
 * storing, showing, importing, and deleting passwords securely using GPG encryption.
 */
class Pass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password_store  MEMBER m_password_store READ password_store WRITE set_password_store )
    Q_PROPERTY(QString gpg_home  MEMBER m_gpg_home READ gpg_home WRITE set_gpg_home )

private slots:
    /**
     * @brief Slot to handle the result of a GPG decryption operation (to show password).
     * @param err The error that occurred during the operation.
     * @param plain_text The decrypted plain text (password).
     */
    void slotShowError(rnp_result_t err);

    void slotShowSucceed(QString encrypted_file_path, QString plain_text);

    /**
     * @brief Slot to handle the result of a GPG key deletion operation.
     * @param err The error that occurred during the operation.
     */
    // void deleteGPGKeyResult(Error err);

    /**
     * @brief Slot to handle the error result of a GPG key import operation.
     * @param err The error that occurred during the operation.
     */
    void slotImportGPGKeyError(rnp_result_t err);

    /**
     * @brief Slot to handle the succeed result of a GPG key import operation.
     */
    void slotImportGPGKeySucceed();

    /**
     * @brief Slot to handle the result of retrieving all GPG keys.
     * @param err The error that occurred during the operation.
     */
    void slotGetAllGPGKeysError(rnp_result_t err);

    /**
     * @brief Slot to handle the succeed result of a GPG key get all keys operation.
     */
    void slotGetAllGPGKeysSucceed(QList<QJsonDocument> result);

    /**
     * @brief Slot to handle the result of a delete Password Store operation.
     * @param err True if an error occurred during the operation.
     */
    void deletePasswordStoreResult(bool err);

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
    void getAllGPGKeysSucceed(QObject* keys_info);

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

    /**
     * @brief Emitted hen showing a password cancelled.
     */
    void showCancelled();


    /**
     * @brief Emitted when the password store is successfully deleted.
     */
    void deletePasswordStoreSucceed();

    /**
     * @brief Emitted when deleting the password store fails.
     * @param message The error message describing the failure.
     */
    void deletePasswordStoreFailed(QString message);

private:
    QString m_password_store; /**< The path to the password store. */
    QString m_gpg_home; /**< The path to the gpg home. */
    std::unique_ptr<PassKeyringModel> m_keyring_model; /**< Meta data on the keyring uid, name, secrecy ... of the availble keys. */
    rnp_password_cb m_passphrase_provider; /**< Pointer on passphrase povider for operations using secret keys. */
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */


    /**
     * @brief Initialize gpg home.
     */
    void initGpgHome();

    /**
     * @brief Initialize password store.
     */
    void initPasswordStore();

public:
    /**
     * @brief Constructs the Pass object.
     */
    Pass();

    /**
     * @brief Gets the path to the password store.
     * @return The path to the password store.
     */
    QString password_store() const
    {
        return this->m_password_store;
    };

    /**
     * @brief Set the path to the password store.
     * @param The path to the password store.
     */
    void set_password_store(QString password_store)
    {
        qInfo() << "[Pass] Password Store changed to :" << password_store;
        this->m_password_store = password_store;
    };

    /**
     * @brief Gets the path to the gpg home.
     * @return The path to the gpg home.
     */
    QString gpg_home() const
    {
        return this->m_gpg_home;
    };

    /**
     * @brief Set the path to the gpg hom.
     * @param The path to the gpg hom
     */
    void set_gpg_home(QString gpg_home)
    {
        qInfo() << "[Pass] GPG Home changed to :" << gpg_home;
        this->m_gpg_home = gpg_home;
    };


    void set_passphrase_provider(rnp_password_cb passphrase_provider)
    {
        this->m_passphrase_provider = passphrase_provider;
    }

    /**
     * @brief Initializes the Pass object with the given window.
     * @param window The QObject window to interact with.
     */
    Q_INVOKABLE void initialize(QObject *window);

    // GPG-related methods

    /**
     * @brief Launch the job to delete the specified GPG key.
     * @param key The PassKeyModel to delete.
     * @return True if the job was start successfully, false otherwise.
     */
    Q_INVOKABLE bool deleteGPGKey(PassKeyModel* key);

    /**
     * @brief Launch the job to import a GPG key from the given URL.
     * @param url The URL to import the GPG key from.
     * @return True if the job was start was successfully, false otherwise.
     */
    Q_INVOKABLE bool importGPGKey(QUrl url);

    /**
     * @brief  Launch the to retrieve all GPG keys.
     * @return True if the job was start was successfully, false otherwise.
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
     * @brief Launch the job to shows the password associated with the specified URL.
     * @param url The URL pointing to the password store entry.
     * @return True if the job was start successfully, false otherwise.
     */
    Q_INVOKABLE bool show(QUrl url);

    /**
     * @brief Launch the job to delete the password store.
     * @return True if  if the job was start successfully, false otherwise.
     */
    Q_INVOKABLE bool deletePasswordStore();
};

#endif
