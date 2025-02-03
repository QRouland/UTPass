#ifndef DELETEKEYJOB_H
#define DELETEKEYJOB_H

#include "rnpjob.h"

/**
 * @class DeleteKeyJob
 * @brief A job to handle the deletion of a key in a separate thread.
 *
 */
class DeleteKeyJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief Executes the key deletion operation.
     *
     * This function performs the actual process of deleting the GPG key from the keyring.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when the key deletion operation is successful.
     *
     * This signal is emitted when the key is successfully deleted from the keyring..
     */
    void resultSuccess();

private:
    QString m_fingerprint; /**< The fingerprint of the key to delete. */

public:
    /**
     * @brief Constructs a DeleteKeyJob object with the specified fingerprint and keyring directory.
     *
     * This constructor initializes the DeleteKeyJob instance with the directory containing
     * the keyrings and the fingerprint of the GPG key to delete.
     *
     * @param rnp_homedir The directory containing the keyrings where the key will be deleted.
     * @param fingerprint The fingerprint of the key to delete.
     */
    DeleteKeyJob(QDir rnp_homedir, QString fingerprint);
};

#endif // DELETEKEYJOB_H
