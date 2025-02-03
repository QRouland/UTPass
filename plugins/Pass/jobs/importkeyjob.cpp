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
    // rnp_output_t output = NULL;
    // qDebug() << "[ImportKeyJob] Writing pubring to " << this->pubringPath();
    // ret = rnp_output_to_file(&output, this->pubringPath().toLocal8Bit().constData(), RNP_OUTPUT_FILE_OVERWRITE);
    // if (ret == RNP_SUCCESS) {
    //     qDebug() << "[ImportKeyJob] Saving key pubring ";
    //     ret = rnp_save_keys(this->m_ffi, RNP_KEYSTORE_GPG, output, RNP_LOAD_SAVE_PUBLIC_KEYS);

    // }
    // if (ret == RNP_SUCCESS) {
    //     ret = rnp_output_finish(output);
    // }
    // rnp_output_destroy(output);
    // terminateOnError(ret);

    // qDebug() << "[ImportKeyJob] Writing secring to " << this->secringPath();
    // ret = rnp_output_to_file(&output, this->secringPath().toLocal8Bit().constData(), RNP_OUTPUT_FILE_OVERWRITE);
    // if (ret == RNP_SUCCESS) {
    //     qDebug() << "[ImportKeyJob] Saving key secring ";
    //     ret = rnp_save_keys(this->m_ffi, RNP_KEYSTORE_GPG, output, RNP_LOAD_SAVE_SECRET_KEYS);
    // }

    // rnp_output_destroy(output);
    // terminateOnError(ret);

    // Emit result
    emit resultSuccess();
    qDebug() << "[ImportKeyJob] Finished Successfully ";
}
