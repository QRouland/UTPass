#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSet>
#include "qjsonarray.h"
#include "rnpjob.h"
extern "C" {
#include <rnp/rnp.h>
#include <rnp/rnp_err.h>
}

RnpJob::RnpJob(QDir rnp_homedir):
    m_rnp_homedir(rnp_homedir)
{
    qRegisterMetaType<rnp_result_t>("rnp_result_t");
    qRegisterMetaType<QList<QJsonDocument >> ("QList<QJsonDocument>");
    qRegisterMetaType<QString *>("QString*");

    auto ret = rnp_ffi_create(&this->m_ffi,
                              RNP_KEYSTORE_GPG,
                              RNP_KEYSTORE_GPG);
    if (ret != RNP_SUCCESS) {
        qDebug() << "[RnpJob] Err : " << ret;
        qFatal("Error on rnp ffi init!");
    }
}

RnpJob::~RnpJob()
{
    auto ret = rnp_ffi_destroy(this->m_ffi);
    if (ret != RNP_SUCCESS) {
        qDebug() << "[RnpJob] Err : " << ret;
        qFatal("Something go wrong on rnp ffi detroy");
    }
}

bool RnpJob::passProvider(rnp_ffi_t        ffi,
                          void            *app_ctx,
                          rnp_key_handle_t key,
                          const char      *pgp_context,
                          char             buf[],
                          size_t           buf_len)
{
    if (strcmp(pgp_context, "protect")) {
        return false;
    }

    strncpy(buf, "password", buf_len);
    return true;
}


void RnpJob::loadKeyFile(QSet<QString> *result_fingerprints, const QString path, const uint32_t flags)
{
    qDebug() << "[RnpJob] Load keyring at" << path;
    rnp_input_t input = NULL;
    if (QFileInfo::exists(this->pubringPath())) {
        auto ret = rnp_input_from_path(&input, path.toLocal8Bit().constData());
        char *json = NULL;
        if (ret == RNP_SUCCESS) {
            ret = rnp_import_keys(this->m_ffi,
                                  input,
                                  flags,
                                  &json);
        }
        QJsonDocument json_document = QJsonDocument::fromJson(json);
        qDebug() << "[RnpJob] json" << json_document;
        if (result_fingerprints) {
            foreach (const QJsonValue fingerprint,  json_document.object()["keys"].toArray()) {
                qDebug() << "[RnpJob] Add fingerprint" << fingerprint["fingerprint"].toString();
                result_fingerprints->insert(fingerprint["fingerprint"].toString());
            }
        }

        rnp_input_destroy(input);
        rnp_buffer_destroy(json);
        terminateOnError(ret);
        qDebug() << "[RnpJob] Keyring loaded successfully";
    } else {
        qDebug() << "[RnpJob] Keyring" << path << "not found";
    }
}


void RnpJob::loadPubKeyring(QSet<QString> *result_fingerprints = NULL)
{
    this->loadKeyFile(result_fingerprints, this->pubringPath(), RNP_LOAD_SAVE_PUBLIC_KEYS);
}

void RnpJob::loadSecKeyring(QSet<QString> *result_fingerprints = NULL)
{
    this->loadKeyFile(result_fingerprints, this->secringPath(),  RNP_LOAD_SAVE_SECRET_KEYS);
}

void RnpJob::loadFullKeyring(QSet<QString> *result_fingerprints = NULL)
{
    this->loadPubKeyring(result_fingerprints);
    this->loadSecKeyring(result_fingerprints);
}


void RnpJob::saveKeyFile(const QString path, const uint32_t flags)
{
    qDebug() << "[RnpJob] Saving keyring at" << path;
    rnp_output_t output = NULL;
    auto ret = rnp_output_to_file(&output, path.toLocal8Bit().data(), RNP_OUTPUT_FILE_OVERWRITE);
    if (ret == RNP_SUCCESS) {
        qDebug() << "[ImportKeyJob] Saving key pubring ";
        ret = rnp_save_keys(this->m_ffi, RNP_KEYSTORE_GPG, output, flags);

    }
    if (ret == RNP_SUCCESS) {
        ret = rnp_output_finish(output);
    }
    rnp_output_destroy(output);
    terminateOnError(ret);
}

void RnpJob::savePubKeyring()
{
    this->saveKeyFile(this->pubringPath(), RNP_LOAD_SAVE_PUBLIC_KEYS);
}

void RnpJob::saveSecKeyring()
{
    this->saveKeyFile(this->secringPath(),  RNP_LOAD_SAVE_SECRET_KEYS);
}

void RnpJob::saveFullKeyring()
{
    this->savePubKeyring();
    this->saveSecKeyring();
}

