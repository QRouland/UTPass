#ifndef RNPJOB_H
#define RNPJOB_H

#include <QThread>
#include <QDir>
extern "C" {
#include <rnp/rnp.h>
}
#include <variant>


#define terminateWithError(ret)                              \
if(ret != RNP_SUCCESS) {                                        \
    qDebug() << "Err : " << ret;                                \
    qDebug() << "Err Msg : " << rnp_result_to_string(ret);      \
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
    void resultSuccess();

private:
    static bool passProvider(rnp_ffi_t        ffi,
                         void *           app_ctx,
                         rnp_key_handle_t key,
                         const char *     pgp_context,
                         char             buf[],
                         size_t           buf_len);
    QDir m_rnp_homedir; ///< rmp ffi.

protected:
    rnp_ffi_t m_ffi; ///< rmp ffi.

    /**
     * @brief Get the path to public keys keyring.
     *
     * @return The path to public keys keyring
     */
    QString pubringPath() {
        return this->m_rnp_homedir.filePath("pubring.pgp");
    }

    /**
     * @brief Get the path to secret keys keyring.
     *
     * @return The path to secret keys keyring
     */
    QString secringPath() {
        return this->m_rnp_homedir.filePath("secring.pgp");
    }


public:
    /**
     * @brief Constructor for the RnpJob class.
     *
     * Initializes the RnpJob instance.
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
