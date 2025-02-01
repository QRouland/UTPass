#ifndef IMPORTKEYJOB_H
#define IMPORTKEYJOB_H

#include "rnpjob.h"

/**
 * @class ImportKeyJob
 * @brief A job to handle the import of a key file in a separate thread.
 *
 */
class ImportKeyJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief Executes the key import operation.
     *
     * This function handles the actual process of importing the GPG key file into the
     * keyring. It is executed in a separate thread to prevent UI freezing or blocking
     * of the main application thread during the import.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when the key import operation is successful.
     *
     * This signal is emitted when the key file is successfully imported into the keyring.
     */
    void resultSuccess();

private:
    QString m_key_file_path; /**< The path of the key file to import. */

public:
    /**
     * @brief Constructs an ImportKeyJob object with the specified key file and keyring directory.
     *
     * This constructor initializes the ImportKeyJob instance with the directory containing
     * the keyrings and the file path of the GPG key to import.
     *
     * @param rnp_homedir The directory containing the keyrings.
     * @param path The path to the key file to import.
     */
    ImportKeyJob(QDir rnp_homedir, QString path);
};

#endif // IMPORTKEYJOB_H
