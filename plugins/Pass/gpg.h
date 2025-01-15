#ifndef GPG_H
#define GPG_H

#include "passkeymodel.h"
#include "passphraseprovider.h"
#include <memory>
#include <QQuickWindow>
#include <gpgme++/context.h>
#include <qgpgme/changeownertrustjob.h>
#include <QSemaphore>

#include <gpgme.h>
#include <qgpgme/importjob.h>
#include <qgpgme/deletejob.h>
#include <qgpgme/decryptjob.h>
#include <qgpgme/encryptjob.h>
#include <qgpgme/protocol.h>
#include <qgpgme/keylistjob.h>
#include <qgpgme/changeownertrustjob.h>


using namespace GpgME;
using namespace QGpgME;

class Gpg: public QObject
{
    Q_OBJECT
    Q_PROPERTY(UTPassphraseProvider* passphrase_provider READ passphrase_provider MEMBER  m_passphrase_provider )

private slots:
    void decryptResultSlot(
        const DecryptionResult &result,
        const QByteArray &plain_text,
        const QString &auditLogAsHtml,
        const Error &auditLogError
    );
    void getKeysJobResultSlot(
        const GpgME::KeyListResult &result,
        const std::vector<GpgME::Key> &keys,
        const QString &auditLogAsHtml ,
        const GpgME::Error &auditLogError
    );

    void importKeysFromFileSlot(
        const GpgME::ImportResult &result,
        const QString &auditLogAsHtml,
        const GpgME::Error &auditLogError
    );

    void deleteKeySlot(
        const GpgME::Error &result,
        const QString &auditLogAsHtml,
        const GpgME::Error &auditLogError
    );

signals:
    void importKeysFromFileResult(Error err);
    void getKeysResult(Error err, std::vector<GpgME::Key> keys);
    void deleteKeyResult(Error err);
    void decryptResult(Error err, QString plain_text);

private:
    UTPassphraseProvider* m_passphrase_provider;

    QString findCommandPath(const QString &command);
    QString initGpgHome();
    QString initGpgExec();
    void initGpgConfig();
    Error getKey(QString uid,  bool remote = false,  bool include_sigs = false,
                 bool validate = false);

public:
    Gpg(QObject* window);
    ~Gpg();

    UTPassphraseProvider* passphrase_provider() const
    {
        return m_passphrase_provider;
    }

    Error importKeysFromFile(const QString path);
    Error getKeys(const QString pattern_uid,  const bool remote = false,
            const bool include_sigs = false,
            const bool validate = false);
    Error getAllKeys (bool remote = false, bool include_sigs = {}, bool
                                                                      validate = false);
    Error deleteKey(const Key key);
    Error decrypt(const QByteArray cipher_text);
    Error decryptFromFile(const QString path);
    //Error encrypt (QString str,  QString uid,  bool ascii_armor = true,
    //                                   bool text_mode = true);

};

#endif
