#include <QDebug>
#include "importkeyjob.h"
extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

ImportKeyJob::ImportKeyJob(QDir rnp_homedir, QString key_file_path):
    RnpJob(rnp_homedir),
    m_key_file_path(key_file_path)
{
    this->setObjectName("DecryptJob");
}


void ImportKeyJob::run()
{
    qDebug() << "ImportKeyJob Starting  ";
    rnp_input_t input = NULL;
    auto ret = rnp_input_from_path(&input, this->m_key_file_path.toLocal8Bit().constData());
    if(ret == RNP_SUCCESS) {
        ret = rnp_load_keys(this->m_ffi,
                  "GPG",
                  input,
                  RNP_LOAD_SAVE_PUBLIC_KEYS | RNP_LOAD_SAVE_SECRET_KEYS);
    }
    rnp_input_destroy(input);
    terminateWithError(ret);

    rnp_output_t output = NULL;
    ret = rnp_output_to_file(&output, this->pubringPath().toLocal8Bit().constData(), RNP_OUTPUT_FILE_RANDOM);
    if(ret == RNP_SUCCESS) {
        ret = rnp_save_keys(this->m_ffi, RNP_KEYSTORE_GPG, output, RNP_LOAD_SAVE_SECRET_KEYS);
    }
    rnp_output_destroy(output);
    terminateWithError(ret);

    ret = rnp_output_to_file(&output, this->secringPath().toLocal8Bit().constData(), RNP_OUTPUT_FILE_OVERWRITE);
    if(ret == RNP_SUCCESS) {
        ret = rnp_save_keys(this->m_ffi, RNP_KEYSTORE_GPG, output, RNP_LOAD_SAVE_SECRET_KEYS);
    }
    rnp_output_destroy(output);
    terminateWithError(ret);
    emit resultSuccess();
    qDebug() << "ImportKeyJob Finished Successfully ";
}
