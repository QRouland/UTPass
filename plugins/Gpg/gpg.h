#ifndef PASS_H
#define PASS_H

#include <QObject>

class Gpg : public QObject
{
    Q_OBJECT

public:
    Gpg();
    ~Gpg() = default;

    Q_INVOKABLE void list_dir();
    Q_INVOKABLE QString get_key_id(QString uid);
    Q_INVOKABLE QStringList get_all_keys_id();
    Q_INVOKABLE bool import_key(QString path);
    Q_INVOKABLE QString decrypt(QByteArray plainText);
    Q_INVOKABLE QString decrypt_file(QString path);
    Q_INVOKABLE QByteArray encrypt(QString str);
    Q_INVOKABLE bool encrypt_file(QString str, QString path);
};

#endif
