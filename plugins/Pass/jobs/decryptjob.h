#ifndef DECRYPTJOB_H
#define DECRYPTJOB_H

#include <QThread>
#include <QDir>

/**
 * @class DecryptJob
 * @brief A class to handle decrypt a file in a separate thread.
 *
 */
class DecryptJob : public QThread
{
    Q_OBJECT

    /**
     * @brief The main function that performs the decrypt operation.
     *
     * Handles the process of removing recursively a target path.
     */
    void run() override;

signals:
    /**
     * @brief Signal emitted when the decrypt operation is complete.
     *
     * @param err A boolean indicating whether an error occurred during removing.
     *        `true` if an error occurred, `false` if the clone was successful.
     */
    void resultReady(const bool err);

private:
    QString m_encryped_file_path; ///< The path of the encrypted file.
    QString m_keyfile_path; ///< The path of the key file.

public:
    /**
     * @brief Constructor for the RmJob class.
     *
     * Initializes the DecryptJob with the specified file to decrypt.
     *
     * @param path Path of the file to decrypt.
     */
    DecryptJob(QString path);
};

#endif // DECRYPTJO_H
