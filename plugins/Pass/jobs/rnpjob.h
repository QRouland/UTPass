#ifndef RNPJOB_H
#define RNPJOB_H

#include <QThread>
#include <QDir>
extern "C" {
#include <rnp/rnp.h>
}
#include <variant>

#define terminateOnError(ret)                              \
if(ret != RNP_SUCCESS) {                                    \
        qDebug() << "[RnpJob] Err : " << ret;                   \
        qDebug() << "[RnpJob] Err Msg : " << rnp_result_to_string(ret); \
        emit resultError(ret);                                  \
        return;                                                 \
} \

/**
 * @class RnpJob
 * @brief A base class that manages OpenPGP-related tasks using the librnp library.
 *
 * The RnpJob class serves as an abstraction for performing OpenPGP (RNP) operations, such as
 * encryption, decryption, and key management, using the RNP library.
 */
class RnpJob : public QThread
{
    Q_OBJECT

signals:
    /**
     * @brief Signal emitted when an error occurs in the RNP job.
     *
     * This signal is emitted when an error occurs during an RNP operation, such as key loading
     * or encryption/decryption failure. The error code is passed to indicate the specific issue.
     *
     * @param err The error code returned by the RNP operation.
     */
    void resultError(const rnp_result_t err);

private:
    /**
     * @brief A callback function for providing the passphrase to RNP.
     *
     * This static function is used as a callback to provide a passphrase to RNP when required
     * during key operations such as decryption or signing. It allows the library to continue the
     * operation with the necessary passphrase.
     *
     * @param ffi The RNP FFI handle.
     * @param app_ctx The application context, used for accessing application-specific data.
     * @param key The key for which the passphrase is required.
     * @param pgp_context The context string (e.g., "decrypt").
     * @param buf The buffer to fill with the passphrase.
     * @param buf_len The length of the buffer.
     *
     * @return true if the passphrase was successfully provided, false otherwise.
     */
    static bool passProvider(rnp_ffi_t        ffi,
                             void            *app_ctx,
                             rnp_key_handle_t key,
                             const char      *pgp_context,
                             char             buf[],
                             size_t           buf_len);

    QDir m_rnp_homedir; /**< Directory that contains the keyrings and RNP configuration. */

    /**
     * @brief Loads a key file into the keyring.
     *
     * This method loads a key file into the keyring, adding keys specified by their fingerprints.
     *
     * @param result_fingerprints A set to hold the fingerprints of the keys loaded into the keyring.
     * @param path The path to the key file.
     * @param flags Flags specifying options for loading keys (e.g., overwrite, secret keys, etc.).
     */
    void loadKeyFile(QSet<QString> *result_fingerprints, const QString path, const uint32_t flags);

    /**
     * @brief Saves keys to the keyring file in the specified directory.
     *
     * This method saves a keyring to a file. It allows you to specify options such as overwriting
     * existing files or including secret keys.
     *
     * @param path The path to the keyring file where the keys should be saved.
     * @param flags Flags specifying options for saving the keys (e.g., overwrite, include secret keys, etc.).
     */
    void saveKeyFile(const QString path, const uint32_t flags);


protected:
    rnp_ffi_t m_ffi; /**< RNP FFI (Foreign Function Interface) handle, used for interacting with the RNP library. */

    /**
     * @brief Get the path to the public keyring.
     *
     * This method returns the file path to the public keyring (where public keys are stored).
     * It combines the directory and file name to provide the full path.
     *
     * @return The path to the public keyring file.
     */
    QString pubringPath()
    {
        return this->m_rnp_homedir.filePath("pubring.pgp");
    }

    /**
     * @brief Get the path to the secret keyring.
     *
     * This method returns the file path to the secret keyring (where private keys are stored).
     * It combines the directory and file name to provide the full path.
     *
     * @return The path to the secret keyring file.
     */
    QString secringPath()
    {
        return this->m_rnp_homedir.filePath("secring.pgp");
    }

    /**
     * @brief Loads the secret keyring into RNP.
     *
     * @param result_fingerprints A set that will hold the fingerprints of the loaded secret keys.
     */
    void loadSecKeyring(QSet<QString> *result_fingerprints);

    /**
     * @brief Loads the public keyring into RNP.
     *
     * @param result_fingerprints A set that will hold the fingerprints of the loaded public keys.
     */
    void loadPubKeyring(QSet<QString> *result_fingerprints);

    /**
     * @brief Loads both the public and secret keyrings into RNP.
     *
     * @param result_fingerprints A set that will hold the fingerprints of all loaded keys.
     */
    void loadFullKeyring(QSet<QString> *result_fingerprints);

    /**
     * @brief Saves the secret keyring to the RNP homedir.
     *
     */
    void saveSecKeyring();

    /**
     * @brief Saves the public keyring to the RNP homedir.
     *
     */
    void savePubKeyring();

    /**
     * @brief Saves both the public and secret keyrings to the RNP homedir.
     *
     */
    void saveFullKeyring();

public:
    /**
     * @brief Constructs an RnpJob object with the specified RNP home directory.
     *
     * This constructor initializes the RnpJob instance with the directory that contains the
     * keyrings and RNP configuration. Keyring files (pubring.pgp and secring.pgp) will be found
     * in this directory.
     *
     * @param rnp_homedir The directory containing the RNP keyrings and configuration files.
     */
    RnpJob(QDir rnp_homedir);

    /**
     * @brief Destructor for the RnpJob class.
     *
     * The destructor cleans up any resources used by the RnpJob instance, including releasing
     * the RNP FFI handle.
     */
    ~RnpJob();


    void setPassProvider(rnp_password_cb pass_provider_cb)
    {
        rnp_ffi_set_pass_provider(this->m_ffi, pass_provider_cb, NULL);
    }
};

#endif // RNPJOB_H
