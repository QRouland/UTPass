#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "jobs/getkeysjob.h"
#include "jobs/importkeyjob.h"
#include "pass.h"



Pass::Pass():
    m_password_store (QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation).append("/.password-store")),
    m_gpg_home (QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation).append("/.rnp")),
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1))),
    m_show_filename(QString())
{

}

void Pass::initialize(QObject *window)
{
    if (!window) {
        qWarning("[Pass] Window should be null only for testing");
    }
    this->initGpgHome();
    this->initPasswordStore();
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

// bool Pass::show(QUrl url)
// {
//     if (!this->m_sem->tryAcquire(1, 500)) {
//         return false;
//     }
//     auto path = url.toLocalFile();
//     qInfo() << "Pass show  " << path;
//     QFileInfo file_info(path);
//     this->m_show_filename = file_info.completeBaseName();
//     return this->m_gpg->decryptFromFile(path);
// }


// void Pass::showResult(Error err, QString plain_text)
// {
//     qDebug() << "Pass show Result";
//     if (err) {
//         qInfo() << "Pass show Failed";
//         emit showFailed(err.asString());

//     }  else if (err.isCanceled()) {
//         qInfo() << "Pass show Cancelled";
//         emit showCancelled();
//     } else {
//         qInfo() << "Pass show Succeed";
//         emit showSucceed(this->m_show_filename, plain_text);
//     }
//     this->m_show_filename = QString();
//     this->m_sem->release(1);
// }

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
        qInfo() << "[Pass] A job is already running";
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
        qInfo() << "[Pass] A job is already running";
        return false;
    }
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
    emit getAllGPGKeysFailed(rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotGetAllGPGKeysSucceed(QSet<QString> result)
{
    qInfo() << "[Pass] Get all GPG Keys Succeed";
    emit getAllGPGKeysSucceed(result.values());
    this->m_sem->release(1);
}

// void Pass::getAllGPGKeysResult(Error err,  std::vector<GpgME::Key> keys_info)
// {
//     qDebug() << "Get GPG keys Result";
//     if (err) {
//         qInfo() << "Get GPG Failed";
//         emit getAllGPGKeysFailed(err.asString());
//     } else {
//         qInfo() << "Get GPG Succeed";
//         emit getAllGPGKeysSucceed(QVariant::fromValue(PassKeyModel::keysToPassKey(keys_info)));
//     }
//     this->m_sem->release(1);
// }

// void Pass::responsePassphraseDialog(bool cancel, QString passphrase)
// {
//     qDebug() << "Propagate responsePassphraseDialog";
//     emit responsePassphraseDialogPropagate(cancel, passphrase);
// }
