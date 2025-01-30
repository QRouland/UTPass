#ifndef GETKEYSJOB_H
#define GETKEYSJOB_H

#include <QJsonDocument>


#include "rnpjob.h"

/**
 * @class GetKeysJob
 * @brief A class to handle get all gpg keys from rings in a separate thread.
 *
 */
class GetKeysJob : public RnpJob
{
    Q_OBJECT

    /**
     * @brief The main function that performs the get all keys operation.
     *
     * Handles the process of removing recursively a target path.
     */
    void run() override;

signals:
    void resultError(const rnp_result_t err);
    void resultSuccess(const QSet<QString> result);

public:
    /**
     * @brief Constructor for the GetKeysJob class.
     *
     * @param rnp_homedir Rnp home dir that contains the keyrings.
     */
    GetKeysJob(QDir rnp_homedir);
};

#endif // GETKEYSJOB_H
