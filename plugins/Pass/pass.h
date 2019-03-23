#ifndef PASS_H
#define PASS_H

#include <QObject>
#include <memory>

class Pass : public QObject
{
    Q_OBJECT
    QString m_password_store;

public:
    Pass();
    ~Pass() override = default;
    Q_INVOKABLE bool gpgImportKeyFromFile(QString path);
    Q_INVOKABLE QStringList gpgListAllKeys();
    Q_INVOKABLE bool gpgSetGpghome(QString path);
};

#endif
