#ifndef PASS_H
#define PASS_H

#include <QObject>
#include <QUrl>
#include <QVariant>
#include <gpgme++/context.h>

#include "gpg.h"

using namespace GpgME;

class Pass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password_store READ password_store MEMBER m_password_store CONSTANT)

private slots:
    void showResult(Error err, QString plain_text);
    void deleteGPGKeyResult(Error err);
    void importGPGKeyResult(Error err);
    void getAllGPGKeysResult(Error err,  std::vector< GpgME::Key > keys_info);

signals:
    // GPG
    void deleteGPGKeySucceed();
    void deleteGPGKeyFailed(QString message);
    void importGPGKeySucceed();
    void importGPGKeyFailed(QString message);
    void getAllGPGKeysSucceed(QVariant keys_info);
    void getAllGPGKeysFailed(QString message);
    void responsePassphraseDialogPropagate(bool cancel, QString passphrase);

    // pass show
    void showSucceed(QString name, QString text);
    void showFailed(QString message);



private:
    QString m_password_store;
    std::unique_ptr<Gpg> m_gpg;
    std::unique_ptr<QSemaphore> m_sem;
    QString m_show_filename;

public:
    Pass();
    ~Pass() override = default;

    QString password_store() const
    {
        return m_password_store;
    }

    Q_INVOKABLE void initialize(QObject *window);

    // GPG
    Q_INVOKABLE bool deleteGPGKey(Key key);
    Q_INVOKABLE bool importGPGKey(QUrl url);
    Q_INVOKABLE bool getAllGPGKeys();
    Q_INVOKABLE void responsePassphraseDialog(bool cancel, QString passphrase);

    // PASS
    Q_INVOKABLE bool show(QUrl url);

};

#endif
