#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

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
    qRegisterMetaType<rnp_result_t>("rnp_result_t");
}

void Pass::initialize(QObject *window)
{
    if (!window) {
        qWarning("Window should not be null unless your in testing");
    }

    // this->m_gpg = std::unique_ptr<Gpg>(new Gpg(window));
    // UTPassphraseProvider *passphrase_provider = dynamic_cast<UTPassphraseProvider*>(this->m_gpg->passphrase_provider());
    // QObject::connect(this, &Pass::responsePassphraseDialogPropagate, passphrase_provider,
    //                  &UTPassphraseProvider::handleResponse);

    // QObject::connect(this->m_gpg.get(), &Gpg::getKeysResult, this, &Pass::getAllGPGKeysResult);
    // QObject::connect(this->m_gpg.get(), &Gpg::deleteKeyResult, this, &Pass::deleteGPGKeyResult);
    // QObject::connect(this->m_gpg.get(), &Gpg::decryptResult, this, &Pass::showResult);

    QDir dir(m_password_store);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    qInfo() << "Password Store is :" << m_password_store;
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
    qInfo() << "Import GPG Key from " << url;
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "A job is already running";
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
    qDebug() << "Import GPG Key Failed";
    emit importGPGKeyFailed(rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotImportGPGKeySucceed()
{
    qDebug() << "Import GPG Key Failed";
    emit importGPGKeySucceed();
    this->m_sem->release(1);
}

// bool Pass::getAllGPGKeys()
// {
//     if (!this->m_sem->tryAcquire(1, 500)) {
//         return false;
//     }
//     qInfo() << "Get GPG keys";
//     return this->m_gpg->getAllKeys();
// }

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
