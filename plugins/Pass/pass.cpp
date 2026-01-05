#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QDirIterator>
#include <QtConcurrent/QtConcurrent>
#include "error.h"
#include "jobs/decryptjob.h"
#include "jobs/deletekeyjob.h"
#include "jobs/getkeysjob.h"
#include "jobs/importkeyjob.h"
#include "jobs/rmjob.h"
#include "pass.h"
#include "passphraseprovider.h"



Pass::Pass():
    m_password_store (QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation).append("/.password-store")),
    m_gpg_home (QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation).append("/.rnp")),
    m_passphrase_provider(&UTPassphraseProvider::get_pass_provider),
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1)))
{
    this->initGpgHome();
    this->initPasswordStore();
    QObject::connect(this, &Pass::responsePassphraseDialogPropagate,
                     &UTPassphraseProvider::instance(), &UTPassphraseProvider::handleResponse);
}

void Pass::initialize(QObject *window)
{
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

void Pass::lsJob()
{
    QDirIterator it(this->m_password_store, QStringList() << "*.gpg", QDir::Files, QDirIterator::Subdirectories);
    QList<QString> ret;
    while (it.hasNext()) {
        QFile f(it.next());
        QString fname = f.fileName();
        fname.remove(0, this->m_password_store.length() + 1); // remove system path
        ret.append(fname);
    }
    qInfo() << "[Pass] ls Succeed";
    emit lsSucceed(ret);
    this->m_sem->release(1);
}

bool Pass::ls()
{
    qInfo() << "[Pass] ls";
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    QtConcurrent::run(this, &Pass::lsJob );
    return true;
}

bool Pass::show(QUrl url)
{
    qInfo() << "[Pass] Show";
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
    emit showFailed((int) rnpErrorToErrorCodeShow(err), rnp_result_to_string(err));
    this->m_sem->release(1);
}


void Pass::slotShowSucceed(QString encrypted_file_path, QString plain_text)
{
    qDebug() << "[Pass] Show Succeed";
    QFileInfo file_info(encrypted_file_path);
    emit showSucceed(file_info.completeBaseName(), plain_text);
    this->m_sem->release(1);
}

bool Pass::deletePasswordStore()
{
    qInfo() << "[Pass] Delete Password Store at" << this->m_password_store;
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    auto job = new RmJob(this->m_password_store);
    connect(job, &RmJob::resultReady, this, &Pass::slotDeletePasswordStoreResult);
    connect(job, &RmJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}

void Pass::slotDeletePasswordStoreResult(bool err)
{ 
    if (err) {
        qInfo() << "[Pass] Delete Password Store Failed";
        emit deletePasswordStoreFailed(static_cast<int>(ErrorCode::Error), "Failed to delete password store");
    } else {
        qInfo() << "[Pass] Delete Password Store Succeed";
        this->initPasswordStore(); // reinit an empty password-store
        emit deletePasswordStoreSucceed();
    }
    this->m_sem->release(1);
}


bool Pass::deleteGPGKey(PassKeyModel* key)
{
    qInfo() << "[Pass] Delete GPG key fingerprint " << key->property("keyid").toString();
    if (!this->m_sem->tryAcquire(1, 500)) {
        qInfo() << "[Pass] A command is already running";
        return false;
    }
    auto job = new DeleteKeyJob(this->m_gpg_home, key->property("fingerprint").toString());
    QObject::connect(job, &DeleteKeyJob::resultError, this, &Pass::slotDeleteGPGKeyError);
    QObject::connect(job, &DeleteKeyJob::resultSuccess, this, &Pass::slotDeleteGPGKeySucceed);
    connect(job, &DeleteKeyJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}

void Pass::slotDeleteGPGKeyError(rnp_result_t err)
{
    qInfo() << "[Pass] Delete GPG key Failed";
    emit deleteGPGKeyFailed(static_cast<int>(ErrorCode::Error), rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotDeleteGPGKeySucceed()
{
    qInfo() << "[Pass] Delete GPG key Succesfull";
    emit deleteGPGKeySucceed();
    this->m_sem->release(1);
}


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
    emit importGPGKeyFailed((int) rnpErrorToErrorCodeImportKeyFile(err), rnp_result_to_string(err));
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
    emit getAllGPGKeysFailed(static_cast<int>(ErrorCode::Error), rnp_result_to_string(err));
    this->m_sem->release(1);
}

void Pass::slotGetAllGPGKeysSucceed(QList<QJsonDocument> result)
{
    qInfo() << "[Pass] Get all GPG Keys Succeed";
    this->m_keyring_model = std::unique_ptr<PassKeyringModel>(new PassKeyringModel(result));
    emit getAllGPGKeysSucceed(this->m_keyring_model.get());
    this->m_sem->release(1);
}

void Pass::responsePassphraseDialog(bool cancel, QString passphrase)
{
    qDebug() << "[Pass] Propagate responsePassphraseDialog to UTPassphraseProvider";
    emit responsePassphraseDialogPropagate(cancel, passphrase);
}
