#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "jobs/decryptjob.h"
#include "jobs/getkeysjob.h"
#include "jobs/importkeyjob.h"
#include "pass.h"
#include "passphraseprovider2.h"
//#include "passphraseprovider2.h"



Pass::Pass():
    m_password_store (QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation).append("/.password-store")),
    m_gpg_home (QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation).append("/.rnp")),
    m_passphrase_provider(&UTPassphraseProvider::get_pass_provider),
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1)))
{}

void Pass::initialize(QObject *window)
{
    this->initGpgHome();
    this->initPasswordStore();
    if (!window) {
        qWarning("[Pass] Window should be null only for testing");
    } else {
        UTPassphraseProvider::instance().setWindow(window);
    }
}


void Pass::initGpgHome()
{
    // delete gpghome from previous version using GPGME
    QString path = QStandardPaths::writableLocation(
                       QStandardPaths::AppDataLocation).append("/.gpghome");
    QDir dir(path);
    dir.removeRecursively();

    // create gpghome for rnp
    QDir dir_gpg_home(this->m_gpg_home);
    if (!dir_gpg_home.exists()) {
        dir_gpg_home.mkpath(".");
    }
    qInfo() << "[Pass] GPG Home is :" << m_gpg_home;
}

void Pass::initPasswordStore()
{
    QDir dir_password_store(this->m_password_store);
    if (!dir_password_store.exists()) {
        dir_password_store.mkpath(".");
    }
    qInfo() << "[Pass] Password Store is :" << m_password_store;
}

bool Pass::show(QUrl url)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    auto job = new DecryptJob(this->m_gpg_home, url.toLocalFile());
    job->setPassProvider(this->m_passphrase_provider);
    QObject::connect(job, &DecryptJob::resultError, this, &Pass::slotShowError);
    QObject::connect(job, &DecryptJob::resultSuccess, this, &Pass::slotShowSucceed);
    connect(job, &DecryptJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}


void Pass::slotShowError(rnp_result_t err)
{
    qInfo() << "[Pass] Show Failed";
    emit showFailed(rnp_result_to_string(err));
    this->m_sem->release(1);
}


void Pass::slotShowSucceed(QString encrypted_file_path, QString plain_text)
{
    qDebug() << "[Pass] Show Succeed";
    QFileInfo file_info(encrypted_file_path);
    emit showSucceed(file_info.completeBaseName(), plain_text);
    this->m_sem->release(1);
}

// bool Pass::deletePasswordStore()
// {
//     if (!this->m_sem->tryAcquire(1, 500)) {
//         return false;
//     }
//     qInfo() << "Pass delete Password Store";
//     auto job = new RmJob(this->password_store());
//     qDebug() << "Delete Password Store at " << this->password_store();
//     connect(job, &RmJob::resultReady, this, &Pass::deletePasswordStoreResult);
//     connect(job, &RmJob::finished, job, &QObject::deleteLater);
//     job->start();
//     return true;
// }

// void Pass::deletePasswordStoreResult(bool err)
// {

//     qDebug() << "Pass delete Password StoreResult";
//     if (err) { //dir.removeRecursively()) {
//         qInfo() << "Pass delete Password Store Failed";
//         emit deletePasswordStoreFailed("failed to delete password store");

//     } else {
//         qInfo() << "Pass delete Password Store Succeed";
//         emit deletePasswordStoreSucceed();
//     }
//     this->m_sem->release(1);
// }


// bool Pass::deleteGPGKey(PassKeyModel* key)
// {
//     if (!this->m_sem->tryAcquire(1, 500)) {
//         return false;
//     }
//     qInfo() << "Delete Key " << key->uid();
//     return this->m_gpg->deleteKey(key->key());
// }

// void Pass::deleteGPGKeyResult(Error err)
// {
//     qDebug() << "Delete Ke yResult";
//     if (err) {
//         qInfo() << "Delete Key Failed";
//         emit deleteGPGKeyFailed(err.asString());
//     } else {
//         qInfo() << "Delete Key Succeed";
//         emit deleteGPGKeySucceed();
//     }
//     this->m_sem->release(1);
// }

bool Pass::importGPGKey(QUrl url)
{
    qInfo() << "[Pass] Import GPG Key from " << url;
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    auto job = new ImportKeyJob(this->m_gpg_home, url.toLocalFile());
    QObject::connect(job, &ImportKeyJob::resultError, this, &Pass::slotImportGPGKeyError);
    QObject::connect(job, &ImportKeyJob::resultSuccess, this, &Pass::slotImportGPGKeySucceed);
    connect(job, &ImportKeyJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}

void Pass::slotImportGPGKeyError(rnp_result_t err)
{
    qInfo() << "[Pass] Import GPG Key Failed";
    emit importGPGKeyFailed(rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotImportGPGKeySucceed()
{
    qInfo() << "[Pass] Import GPG Key Succesfull";
    emit importGPGKeySucceed();
    this->m_sem->release(1);
}

bool Pass::getAllGPGKeys()
{
    qInfo() << "[Pass] Get all GPG Keys";
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    this->m_keyring_model = nullptr;
    auto job = new GetKeysJob(this->m_gpg_home);
    QObject::connect(job, &GetKeysJob::resultError, this, &Pass::slotGetAllGPGKeysError);
    QObject::connect(job, &GetKeysJob::resultSuccess, this, &Pass::slotGetAllGPGKeysSucceed);
    connect(job, &ImportKeyJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}

void Pass::slotGetAllGPGKeysError(rnp_result_t err)
{
    qInfo() << "[Pass] Get all GPG Keys Failed";
    this->m_keyring_model = nullptr;
    emit getAllGPGKeysFailed(rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotGetAllGPGKeysSucceed(QList<QJsonDocument> result)
{
    qInfo() << "[Pass] Get all GPG Keys Succeed";
    this->m_keyring_model = std::unique_ptr<PassKeyringModel>(new PassKeyringModel(result));
    emit getAllGPGKeysSucceed(this->m_keyring_model.get());
    this->m_sem->release(1);
}

// void Pass::responsePassphraseDialog(bool cancel, QString passphrase)
// {
//     qDebug() << "Propagate responsePassphraseDialog";
//     emit responsePassphraseDialogPropagate(cancel, passphrase);
// }
