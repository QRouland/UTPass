#ifndef PASS_H
#define PASS_H

#include <QObject>

class Pass: public QObject {
    Q_OBJECT
    QString gpgHome;

public:
    Pass();
    ~Pass() override = default;

    Q_INVOKABLE QString decrypt(QByteArray plainText);
    Q_INVOKABLE QString decrypt_file(QString path);
    Q_INVOKABLE QByteArray encrypt(QString str);
    Q_INVOKABLE bool encrypt_file(QString str, QString path);
};

#endif
