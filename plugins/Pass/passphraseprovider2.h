#ifndef UTPASSPHRASEPROVIDER2_H
#define UTPASSPHRASEPROVIDER2_H

#include <QDebug>
#include <memory>
#include <stdio.h>
#include <QObject>
#include <QQmlProperty>
#include <QEventLoop>
#include <QSemaphore>
extern "C" {
#include <rnp/rnp.h>
}

/**
 * @class UTPassphraseProvider
 * @brief A passphrase provider for GPG operations that interacts with a QML dialog.
 */

class UTPassphraseProvider : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief Slot to handle the user's response from the passphrase dialog.
     *
     * This method processes the response from the passphrase dialog. If the user provides a passphrase,
     * it is stored; if the operation is canceled, a flag is set.
     *
     * @param canceled Whether the user canceled the passphrase entry.
     * @param passphrase The passphrase entered by the user.
     */
    void handleResponse(bool canceled, QString passphrase)
    {
        qDebug() << "call handleResponse";
        if (!canceled)
           this->m_passphrase = passphrase;
        else
            m_canceled = true;
        emit unlockEventLoop();
    };

signals:
    /**
     * @brief Signal to unlock the event loop.
     *
     * This signal is emitted when the passphrase has been entered or the operation has been canceled,
     * unlocking the event loop waiting for the response.
     */
    void unlockEventLoop();


private:
    explicit UTPassphraseProvider(QObject * parent = nullptr)
        : m_sem(std::make_unique<QSemaphore>(1)),
          m_passphrase(QString::Null()),
          m_canceled(false)
    {}
    QObject *m_window; /**< The window object that triggers the QML dialog. */
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing access. */
    QString m_passphrase; /**< The passphrase provided by the user. */
    bool m_canceled; /**< Flag indicating whether the passphrase operation was canceled. */

public:
    ~UTPassphraseProvider() = default;

    static UTPassphraseProvider& instance()
    {
        static UTPassphraseProvider instance;
        return instance;
    }
    UTPassphraseProvider(UTPassphraseProvider const &) = delete;
    void operator=(UTPassphraseProvider const &) = delete;

    static bool
    get_pass_provider(rnp_ffi_t        ffi,
                          void *           app_ctx,
                          rnp_key_handle_t key,
                          const char *     pgp_context,
                          char             buf[],
                          size_t           buf_len)
    {
        qDebug() << "Call the getPassphrase";

        if (!UTPassphraseProvider::instance().m_sem->tryAcquire(1, 500))
        {
            qWarning() << "Cannot acquire UTPassphraseProvider semaphore.";
            UTPassphraseProvider::instance().m_canceled = true;
            return false;
        }

        UTPassphraseProvider::instance().m_passphrase = nullptr;
        UTPassphraseProvider::instance().m_canceled = false;

        qDebug() << "Call the QML Dialog Passphrase Provider";
        QMetaObject::invokeMethod(
            UTPassphraseProvider::instance().m_window, "callPassphraseDialog",
            Q_ARG(QVariant, "useridHint"),      // TODO
            Q_ARG(QVariant, "description"),     // TODO
            Q_ARG(QVariant, "previousWasBad")   // TODO
            );

        qDebug() << "Waiting for response";

        QEventLoop loop;
        QObject::connect(&UTPassphraseProvider::instance(), &UTPassphraseProvider::unlockEventLoop, &loop, &QEventLoop::quit);
        loop.exec();

        qDebug() << "Prepare Returns";
        auto ret = false;
        if(!UTPassphraseProvider::instance().m_canceled) {
            strncpy(buf, UTPassphraseProvider::instance().m_passphrase.toLocal8Bit().data(), buf_len);
            ret = true;
        };

        qDebug() << "Clean";
        if (UTPassphraseProvider::instance().m_passphrase.isNull())
        {
            UTPassphraseProvider::instance().m_passphrase = QString::Null();
        }
        UTPassphraseProvider::instance().m_canceled = false;
        UTPassphraseProvider::instance().m_sem->release(1);
        return ret;
    }

    void setWindow(QObject* window){
         this->m_window = window;
    }
};

#endif
