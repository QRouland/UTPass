#ifndef RNPJOB_H
#define RNPJOB_H

#include <QThread>
#include <QDir>
extern "C" {
#include <rnp/rnp.h>
}
#include <variant>


#define terminateOnError(ret)                              \
if(ret != RNP_SUCCESS) {                                        \
    qDebug() << "[RnpJob] Err : " << ret;                                \
    qDebug() << "[RnpJob] Err Msg : " << rnp_result_to_string(ret);      \
    emit resultError(ret);                                      \
    return;                                                     \
}                                                               \

/**
 * @class RmpJob
 * @brief A class that manages Git-related tasks using libgit2.
 *
 * The GitJob class is used abstraction class to perform rnp (opengpg) operations,
 * such as decrypt, encrypt, key managments operations
 */
class RnpJob : public QThread
{
    Q_OBJECT

signals:
    void resultError(const rnp_result_t err);

private:
    static bool passProvider(rnp_ffi_t        ffi,
                             void            *app_ctx,
                             rnp_key_handle_t key,
                             const char      *pgp_context,
                             char             buf[],
                             size_t           buf_len);
    QDir m_rnp_homedir; ///< rmp ffi.
    void load_key_file(QSet<QString> *fingerprints, const QString path, const uint32_t flags);

protected:
    rnp_ffi_t m_ffi; ///< rmp ffi.

    /**
     * @brief Get the path to public keys keyring.
     *
     * @return The path to public keys keyring
     */
    QString pubringPath()
    {
        return this->m_rnp_homedir.filePath("pubring.pgp");
    }

    /**
     * @brief Get the path to secret keys keyring.
     *
     * @return The path to secret keys keyring
     */
    QString secringPath()
    {
        return this->m_rnp_homedir.filePath("secring.pgp");
    }

    void load_sec_keyring(QSet<QString> *fingerprints);
    void load_pub_keyring(QSet<QString> *fingerprints);
    void load_full_keyring(QSet<QString> *fingerprints);

public:
    /**
     * @brief Constructor for the RnpJob class.
     *
     * Initializes the RnpJob instance.
     *
     * @param rnp_homedir Rnp home dir that contains the keyrings.
     */
    RnpJob(QDir rnp_homedir);

    /**
     * @brief Destructor for the RnpJob class.
     *
     * Cleans up any resources used by the RnpJob.
     */
    ~RnpJob();
};

#endif // RNPJOB_H
