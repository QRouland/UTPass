#ifndef IMPORTKEYJOB_H
#define IMPORTKEYJOB_H



#include "rnpjob.h"
/**
 * @class ImportKeyJob
 * @brief A class to handle import a key file in a separate thread.
 *
 */
class ImportKeyJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief The main function that performs the import operation.
     *
     * Handles the process of removing recursively a target path.
     */
    void run() override;

signals:
    void resultSuccess();

private:
    QString m_key_file_path; ///< The path of the key file to import.

public:
    /**
     * @brief Constructor for the ImportKeyJob class.
     *
     * Initializes the ImportKeyJob with the file to import.
     *
     * @param rnp_homedir Rnp home dir that contains the keyrings.
     * @param path Path of the key file to import.
     */
    ImportKeyJob(QDir rnp_homedir, QString path);
};

#endif // IMPORTKEYJOB_H
