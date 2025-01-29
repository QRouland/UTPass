#include <QDebug>

#include "rnpjob.h"
extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

RnpJob::RnpJob(QDir rnp_homedir):
    m_rnp_homedir(rnp_homedir)
{
    auto ret = rnp_ffi_create(&this->m_ffi,
                   RNP_KEYSTORE_GPG,
                   RNP_KEYSTORE_GPG);
    if(ret != RNP_SUCCESS) {
        qDebug() << "Err : " << ret;
        qFatal("Error on rnp ffi init!");
    }
}

RnpJob::~RnpJob(){
    auto ret = rnp_ffi_destroy(this->m_ffi);
    if(ret != RNP_SUCCESS) {
        qDebug() << "Err : " << ret;
        qFatal("Something go wrong on rnp ffi detroy");
    }
}

bool RnpJob::passProvider(rnp_ffi_t        ffi,
                      void *           app_ctx,
                      rnp_key_handle_t key,
                      const char *     pgp_context,
                      char             buf[],
                      size_t           buf_len)
{
    if (strcmp(pgp_context, "protect")) {
        return false;
    }

    strncpy(buf, "password", buf_len);
    return true;
}
