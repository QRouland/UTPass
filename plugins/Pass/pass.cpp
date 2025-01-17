#include <QUrl>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

#include "pass.h"
#include "gpg.h"
#include "passkeymodel.h"



Pass::Pass():
    m_password_store (QStandardPaths::writableLocation(
                          QStandardPaths::AppDataLocation).append("/.password-store")),
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1))),
    m_show_filename(QString())
{}

void Pass::initialize(QObject *window)
{
    if (!window) {
        qFatal("window is invalid. Abording.");
    }

    this->m_gpg = std::unique_ptr<Gpg>(new Gpg(window));

    QObject::connect(this, &Pass::responsePassphraseDialogPropagate, this->m_gpg->passphrase_provider(),
                     &UTPassphraseProvider::handleResponse);
    QObject::connect(this->m_gpg.get(), &Gpg::importKeysFromFileResult, this, &Pass::importGPGKeyResult);
    QObject::connect(this->m_gpg.get(), &Gpg::getKeysResult, this, &Pass::getAllGPGKeysResult);
    QObject::connect(this->m_gpg.get(), &Gpg::deleteKeyResult, this, &Pass::deleteGPGKeyResult);
    QObject::connect(this->m_gpg.get(), &Gpg::decryptResult, this, &Pass::showResult);

    QDir dir(m_password_store);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    qInfo() << "Password Store is :" << m_password_store;
}

bool Pass::show(QUrl url)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        return false;
    }
    auto path = url.toLocalFile();
    qInfo() << "Staring decrypting job for " << path;
    QFileInfo file_info(path);
    this->m_show_filename = file_info.completeBaseName();
    return this->m_gpg->decryptFromFile(path);
}

void Pass::showResult(Error err, QString plain_text)
{
    qInfo() << "Result for decrypting job";
    if (err) {
        qInfo() << "Decrypt Failed";
        emit showFailed(err.asString());

    }  else if (err.isCanceled()){
            qInfo() << "Decrypt Cancelled";
            emit showCancelled();
    } else {
        qInfo() << "Decrypt OK";
        emit showSucceed(this->m_show_filename, plain_text);
    }
    this->m_show_filename = QString();
    this->m_sem->release(1);
}

bool Pass::deleteGPGKey(Key key)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        return false;
    }
    qInfo() << "Deleting Key";
    return this->m_gpg->deleteKey(key);
}

void Pass::deleteGPGKeyResult(Error err)
{
    if (err) {
        emit deleteGPGKeyFailed(err.asString());
    } else {
        emit deleteGPGKeySucceed();
    }
    this->m_sem->release(1);
}

bool Pass::importGPGKey(QUrl url)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        return false;
    }
    qInfo() << "Importing Key from " << url;
    return this->m_gpg->importKeysFromFile(url.toLocalFile());
}

void Pass::importGPGKeyResult(Error err)
{
    if (err) {
        emit importGPGKeyFailed(err.asString());
    } else {
        emit importGPGKeySucceed();
    }
    this->m_sem->release(1);
}

bool Pass::getAllGPGKeys()
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        return false;
    }
    qInfo() << "Getting all key form gpg ";
    return this->m_gpg->getAllKeys();
}

void Pass::getAllGPGKeysResult(Error err,  std::vector<GpgME::Key> keys_info)
{
    if (err) {
        emit getAllGPGKeysFailed(err.asString());
    } else {
        emit getAllGPGKeysSucceed(QVariant::fromValue(PassKeyModel::keysToPassKey(keys_info)));
    }
    this->m_sem->release(1);
}

void Pass::responsePassphraseDialog(bool cancel, QString passphrase)
{
    qDebug() << "responsePassphraseDialog";
    emit responsePassphraseDialogPropagate(cancel, passphrase);
}
