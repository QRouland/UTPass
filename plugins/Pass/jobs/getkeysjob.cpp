#include <QDebug>
#include "getkeysjob.h"
#include <QJsonDocument>
#include <QJsonObject>

extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

GetKeysJob::GetKeysJob(QDir rnp_homedir):
    RnpJob(rnp_homedir)
{
    this->setObjectName("GetKeysJob");
}


QJsonDocument GetKeysJob::fingerprint_map_key_info(const QString fingerprint)
{
    rnp_key_handle_t handle;
    rnp_locate_key(this->m_ffi, "fingerprint", fingerprint.toLocal8Bit().data(), &handle);
    char *result;
    rnp_key_to_json( handle, 0, &result);
    return QJsonDocument::fromJson(result);
}

void GetKeysJob::run()
{
    qDebug() << "[GetKeysJob] Starting";

    // Loading keyring
    QSet<QString> fingerprints = QSet<QString>();
    this->load_full_keyring(&fingerprints);

    auto key_infos = QList<QJsonDocument>();
    QList<QJsonDocument>::iterator i;
    for (auto i = fingerprints.begin(), end = fingerprints.end(); i != end; ++i) {
        key_infos.append(this->fingerprint_map_key_info(*i));
    }

    //Get all infos keys
    emit resultSuccess(key_infos);
    qDebug() << "[GetKeysJob] Finished Successfully ";
}
