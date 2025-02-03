#ifndef DECRYPTJOB_H
#define DECRYPTJOB_H

#include "rnpjob.h"
#include <QThread>
#include <QDir>

/**
 * @class DecryptJob
 * @brief A job to handle the decryption of a file in a separate thread.
 *
 */
class DecryptJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief Executes the decryption operation.
     *
     * This method performs the actual decryption of the encrypted file specified during
     * object construction.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when the decryption operation is complete.
     *
     * This signal is emitted once the decryption operation finishes, providing the results.
     * It indicates whether the decryption was successful and provides the clear-text output
     * if the decryption was successful.
     *
     * @param encrypted_file_path The path to the encrypted file that was decrypted.
     * @param clear_txt The decrypted content in clear-text. If an error occurs, this may be empty.
     */
    void resultSuccess(QString encrypted_file_path, QString clear_txt);

private:
    QString m_encrypted_file_path; /**< The path to the encrypted file that is to be decrypted. */

public:
    /**
     * @brief Constructs a DecryptJob object with the specified encrypted file.
     *
     * This constructor initializes the DecryptJob with the encrypted file path. The decryption
     * operation will be executed in a background thread when the job is started.
     *
     * @param rnp_homedir The directory containing the keyrings.
     * @param path The path to the encrypted file that needs to be decrypted.
     */
    DecryptJob(QDir rnp_homedir, QString path);
};

#endif // DECRYPTJOB_H
