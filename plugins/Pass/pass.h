#ifndef PASS_H
#define PASS_H

#include <QObject>
#include <QUrl>
#include <QVariant>


class Pass : public QObject
{
    Q_OBJECT
    QString m_password_store;

signals:
    void decrypted(QString text);
    void decryptCanceled();
    void decryptFailed();


public:
    Pass();
    ~Pass() override = default;

    Q_INVOKABLE void init(QObject *window);
    Q_INVOKABLE QString getPasswordStore();
    Q_INVOKABLE void decrypt(QUrl url);
    Q_INVOKABLE bool gpgDeleteKeyId(QString id);
    Q_INVOKABLE bool gpgImportKeyFromFile(QUrl url);
    Q_INVOKABLE QVariant gpgGetAllKeysModel();
};

#endif
