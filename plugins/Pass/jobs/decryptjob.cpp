#include "decryptjob.h"
#include "qdebug.h"
extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

DecryptJob::DecryptJob(QDir rnp_homedir, QString path):
    RnpJob(rnp_homedir),
    m_encrypted_file_path(path)
{
    this->setObjectName("DecryptJob");
}


void DecryptJob::run()
{
    qDebug() << "[DecryptJob] Starting";
    this->loadFullKeyring(NULL);

    rnp_input_t  input = NULL;
    rnp_output_t output = NULL;
    uint8_t     *buf = NULL;
    size_t       buf_len = 0;
    QString data = QString::Null();

    auto ret = rnp_input_from_path(&input, this->m_encrypted_file_path.toLocal8Bit().data());
    if (ret == RNP_SUCCESS) {
        ret = rnp_output_to_memory(&output, 0);
    }
    if (ret == RNP_SUCCESS) {
        ret = rnp_decrypt(this->m_ffi, input, output);
    }
    if (ret == RNP_SUCCESS) {
        ret = rnp_output_memory_get_buf(output, &buf, &buf_len, false);
    }
    if (ret == RNP_SUCCESS) {
        data = QString::fromUtf8((char*)buf, buf_len);
    }

    rnp_input_destroy(input);
    rnp_output_destroy(output);

    terminateOnError(ret);
    emit resultSuccess(this->m_encrypted_file_path, data);
    qDebug() << "[DecryptJob] Finished Successfully ";
}
