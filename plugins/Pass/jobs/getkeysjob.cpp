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



void GetKeysJob::run()
{
    qDebug() << "[GetKeysJob] Starting";

    // Loading keyring
    QSet<QString> fingerprints = QSet<QString>();
    this->load_full_keyring(&fingerprints);

    //Get all infos keys
    emit resultSuccess(fingerprints);
    qDebug() << "[GetKeysJob] Finished Successfully ";
}
