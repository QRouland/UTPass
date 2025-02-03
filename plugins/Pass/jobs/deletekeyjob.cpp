#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include "deletekeyjob.h"

extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

DeleteKeyJob::DeleteKeyJob(QDir rnp_homedir, QString fingerprint):
    RnpJob(rnp_homedir),
    m_fingerprint(fingerprint)
{
    this->setObjectName("ImportKeyJob");
}


void DeleteKeyJob::run()
{
    qDebug() << "[DeleteKeyJob] Starting";

    // Loading keyring
    this->loadFullKeyring(NULL);

    // Delete key
    rnp_key_handle_t key = NULL;
    auto ret = rnp_locate_key(this->m_ffi, "fingerprint", this->m_fingerprint.toLocal8Bit().data(), &key);

    if (ret == RNP_SUCCESS) {
        ret = rnp_key_remove(key, RNP_KEY_REMOVE_PUBLIC | RNP_KEY_REMOVE_SECRET | RNP_KEY_REMOVE_SUBKEYS);
    };
    rnp_key_handle_destroy(key);

    // Save resulting keyring
    this->saveFullKeyring();

    // Emit result
    terminateOnError(ret);
    emit resultSuccess();
    qDebug() << "[DeleteKeyJob] Finished Successfully ";
}
