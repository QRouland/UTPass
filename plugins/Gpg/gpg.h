#ifndef PASS_H
#define PASS_H

#include <QObject>

class Gpg : public QObject
{
    Q_OBJECT

public:
    Gpg();
    ~Gpg() override = default;

    Q_INVOKABLE void listDir();
    Q_INVOKABLE QString getKeyId(QString uid);
    Q_INVOKABLE QStringList getAllKeysId();
    Q_INVOKABLE bool importKey(QString path);
    Q_INVOKABLE QString decrypt(QByteArray plainText);
    Q_INVOKABLE QString decryptFile(QString path);
    Q_INVOKABLE QByteArray encrypt(QString str);
    Q_INVOKABLE bool encryptFile(QString str, QString path);
};

#endif
