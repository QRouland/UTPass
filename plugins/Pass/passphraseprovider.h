#ifndef UTPASSPHRASEPROVIDER_H
#define UTPASSPHRASEPROVIDER_H

#include <stdio.h>
#include <QObject>
#include <QQmlProperty>
#include <QEventLoop>
#include <QSemaphore>
#include <gpgme++/interfaces/passphraseprovider.h>
#include "passphraseprovider.h"
#include "gpg.h"



class UTPassphraseProvider : public QObject, public PassphraseProvider
{
    Q_OBJECT
private:
    std::unique_ptr<QEventLoop> m_loop;
    std::unique_ptr<QSemaphore> m_sem;
    char *m_passphrase;
    bool m_canceled;

public slots:
    void handleResponse(bool canceled, QString p)
    {
        if (!canceled)
            gpgrt_asprintf(&m_passphrase, "%s", p.toUtf8().constData());
        else
            m_canceled = true;
        m_loop->quit();
    };


public:
    UTPassphraseProvider():
        m_loop(std::unique_ptr<QEventLoop>(new QEventLoop)),
        m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1))),
        m_passphrase(nullptr),
        m_canceled(false)
    {}

    char *getPassphrase( const char *useridHint,
                         const char *description,
                         bool previousWasBad,
                         bool &canceled ) Q_DECL_OVERRIDE {
        if (!m_sem->tryAcquire(1, 3000))
        {
            qWarning() << "Cannot acquire UTPassphraseProvider semaphore.";
            canceled = true;
            return nullptr;
        }

        m_passphrase = nullptr;
        m_canceled = false;

        qDebug() << "Call the QML Dialog Passphrase Provider";
        QMetaObject::invokeMethod(
            Gpg::instance()->getWindow(), "callPassphraseDialog",
            Q_ARG(QVariant, useridHint),
            Q_ARG(QVariant, description),
            Q_ARG(QVariant, previousWasBad)
        );

        qDebug() << "Waiting for response";

        QObject::connect(
            Gpg::instance()->getWindow(), SIGNAL(responsePassphraseDialog(bool, QString)),
            this, SLOT(handleResponse(bool, QString))
        );
        m_loop->exec();

        qDebug() << "Prepare Returns";
        char *ret;
        gpgrt_asprintf(&ret, "%s", m_passphrase);
        canceled = m_canceled;

        qDebug() << "Clean";
        if (m_passphrase)
        {
            free(m_passphrase);
        }
        m_canceled = false;
        m_sem->release(1);
        return ret;
    };
};
#endif
