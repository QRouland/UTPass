#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include "importkeyjob.h"

extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

ImportKeyJob::ImportKeyJob(QDir rnp_homedir, QString key_file_path):
    RnpJob(rnp_homedir),
    m_key_file_path(key_file_path)
{
    this->setObjectName("ImportKeyJob");
}


void ImportKeyJob::run()
{
    qDebug() << "[ImportKeyJob] Starting";

    // Loading keyring
    this->loadFullKeyring(NULL);

    // Import new key
    rnp_input_t input = NULL;
    auto ret = rnp_input_from_path(&input, this->m_key_file_path.toLocal8Bit().constData());
    if (ret == RNP_SUCCESS) {
        char *r = NULL;
        ret = rnp_import_keys(this->m_ffi,
                              input,
                              RNP_LOAD_SAVE_PUBLIC_KEYS | RNP_LOAD_SAVE_SECRET_KEYS,
                              &r);

        qDebug() << "[ImportKeyJob]" << QJsonDocument::fromJson(r);
        rnp_buffer_destroy(r);
    }

    rnp_input_destroy(input);
    terminateOnError(ret);

    // Save resulting keyring
    this->saveFullKeyring();

    // Emit result
    emit resultSuccess();
    qDebug() << "[ImportKeyJob] Finished Successfully ";
}
