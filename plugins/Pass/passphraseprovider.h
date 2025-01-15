#ifndef UTPASSPHRASEPROVIDER_H
#define UTPASSPHRASEPROVIDER_H

#include <QDebug>
#include <stdio.h>
#include <QObject>
#include <QQmlProperty>
#include <QEventLoop>
#include <QSemaphore>
#include <gpgme++/interfaces/passphraseprovider.h>
#include "gpg.h"



class UTPassphraseProvider : public QObject, public PassphraseProvider
{
    Q_OBJECT

public slots:
    void handleResponse(bool canceled, QString p)
        {
            qDebug() << "call handleResponse";
            if (!canceled)
                gpgrt_asprintf(&m_passphrase, "%s", p.toUtf8().constData());
            else
                m_canceled = true;
            emit unlockEventLoop();
        };

signals:
    void unlockEventLoop();

private:
    std::unique_ptr<QSemaphore> m_sem;
    char *m_passphrase;
    bool m_canceled;
    QObject* m_window;


public:
    UTPassphraseProvider(QObject* window):
        m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1))),
        m_passphrase(nullptr),
        m_canceled(false),
        m_window(window)
    {
        qDebug() << "Initialize UTPassphraseProviderr";

    }

    char *getPassphrase( const char *useridHint,
                         const char *description,
                         bool previousWasBad,
                         bool &canceled ) Q_DECL_OVERRIDE {
         qDebug() << "Call the getPassphrase";
        if (!this->m_sem->tryAcquire(1, 500))
        {
            qWarning() << "Cannot acquire UTPassphraseProvider semaphore.";
            canceled = true;
            return nullptr;
        }

        this->m_passphrase = nullptr;
        this->m_canceled = false;

        qDebug() << "Call the QML Dialog Passphrase Provider";
        QMetaObject::invokeMethod(
            this->m_window, "callPassphraseDialog",
            Q_ARG(QVariant, useridHint),
            Q_ARG(QVariant, description),
            Q_ARG(QVariant, previousWasBad)
        );

        qDebug() << "Waiting for response";

        QEventLoop loop;
        QObject::connect(this, &UTPassphraseProvider::unlockEventLoop, &loop, &QEventLoop::quit);
        loop.exec();

        qDebug() << "Prepare Returns";
        char *ret;
        gpgrt_asprintf(&ret, "%s", m_passphrase);
        canceled = this->m_canceled;

        qDebug() << "Clean";
        if (this->m_passphrase)
        {
            free(m_passphrase);
        }
        this->m_canceled = false;
        this->m_sem->release(1);
        return ret;
    };
};
#endif
