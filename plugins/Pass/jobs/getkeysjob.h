#ifndef GETKEYSJOB_H
#define GETKEYSJOB_H

#include <QJsonDocument>
#include "rnpjob.h"

/**
 * @class GetKeysJob
 * @brief A job to retrieve all GPG keys from keyrings in a separate thread.
 *
 */
class GetKeysJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief Executes the process of fetching all GPG keys.
     *
     * This function performs the task of retrieving all keys from the keyrings.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when the key retrieval operation completes successfully.
     *
     * This signal is emitted when the keys are successfully fetched. It passes a list of
     * JSON documents representing the retrieved keys.
     *
     * @param result A list of QJsonDocument objects containing the key information.
     */
    void resultSuccess(const QList<QJsonDocument> result);

private:
    /**
     * @brief Retrieves key information for a specific key fingerprint.
     *
     * This helper function fetches the key data corresponding to the given fingerprint.
     * The returned information is packaged in a JSON document.
     *
     * @param fingerprint The fingerprint of the key to fetch information for.
     * @return A QJsonDocument containing the key's information.
     */
    QJsonDocument fingerprint_map_key_info(const QString fingerprint);

public:
    /**
     * @brief Constructs a GetKeysJob object with the specified keyring directory.
     *
     * This constructor initializes the job with the directory containing the keyrings to
     * search for GPG keys.
     *
     * @param rnp_homedir The directory that contains the keyrings.
     */
    GetKeysJob(QDir rnp_homedir);
};

#endif // GETKEYSJOB_H
