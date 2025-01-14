#ifndef PASS_H
#define PASS_H

#include <QObject>
#include <QUrl>
#include <QVariant>


class Pass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString password_store READ password_store)

private:
    QString m_password_store;

signals:
    void decrypted(QString text);
    void decryptCanceled();
    void decryptFailed();

public:
    Pass();
    ~Pass() override = default;

    QString password_store() const
    {
        return m_password_store;
    }

    Q_INVOKABLE void initialize(QObject *window);
    Q_INVOKABLE void show(QUrl url);
    Q_INVOKABLE bool deleteGPGKey(QString id);
    Q_INVOKABLE bool importGPGKey(QUrl url);
    Q_INVOKABLE QVariant getAllGPGKeys();
};

#endif
