#ifndef UTPASSPHRASEPROVIDER_H
#define UTPASSPHRASEPROVIDER_H

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
 *
 * This class is used to prompt the user for a passphrase through a QML-based dialog. It manages
 * the passphrase entry process and signals whether the user has provided a passphrase or canceled
 * the operation.
 */
class UTPassphraseProvider : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief Slot to handle the user's response from the passphrase dialog.
     *
     * This slot is called when the user provides a passphrase or cancels the passphrase entry.
     * If the user provides a passphrase, it is stored; if the user cancels, a cancellation flag
     * is set.
     *
     * @param canceled Indicates whether the user canceled the passphrase entry.
     * @param passphrase The passphrase entered by the user (if not canceled).
     */
    void handleResponse(bool canceled, QString passphrase)
    {
        qDebug() << "[UTPassphraseProvider] Call handleResponse";
        if (!canceled) {
            this->m_canceled = false;
            this->m_passphrase = passphrase;
        }
        unlockEventLoop();
    };

signals:
    /**
     * @brief Signal to unlock the event loop.
     *
     * This signal is emitted when the passphrase has been entered or the operation has been canceled,
     * causing the event loop waiting for the response to exit.
     */
    void unlockEventLoop();


private:
    /**
     * @brief Private constructor for singleton pattern.
     *
     * Initializes the passphrase provider with a semaphore to manage access, and a flag to indicate
     * whether the operation was canceled.
     *
     * @param parent Parent QObject (default is nullptr).
     */
    explicit UTPassphraseProvider(QObject * parent = nullptr)
        : m_sem(std::make_unique<QSemaphore>(1)),
          m_passphrase(QString::Null()),
          m_canceled(true)
    {}

    QObject *m_window; /**< The window object that triggers the QML dialog. */
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing access to the passphrase entry process. */
    QString m_passphrase; /**< The passphrase provided by the user. */
    bool m_canceled; /**< Flag indicating whether the passphrase operation was canceled. */

public:
    ~UTPassphraseProvider() = default;

    /**
     * @brief Gets the singleton instance of UTPassphraseProvider.
     *
     * This method ensures that only one instance of the passphrase provider exists throughout the application.
     *
     * @return The singleton instance of UTPassphraseProvider.
     */
    static UTPassphraseProvider &instance()
    {
        static UTPassphraseProvider instance;
        return instance;
    }

    UTPassphraseProvider(UTPassphraseProvider const &) = delete; /**< Prevents copying of the instance. */
    void operator=(UTPassphraseProvider const &) = delete; /**< Prevents assignment of the instance. */

    /**
     * @brief Callback function to retrieve the passphrase for GPG operations.
     *
     * This static method is called by the GPG library when it requires a passphrase for a specific key operation.
     * It triggers a QML dialog to prompt the user for the passphrase and waits for a response.
     *
     * @param ffi The RNP FFI instance.
     * @param app_ctx provided by application
     * @param key the key, if any, for which the password is being requested.
     *        Note: this key handle should not be held by the application,
     *        it is destroyed after the callback. It should only be used to
     *        retrieve information like the userids, grip, etc.
     * @param pgp_context a descriptive string on why the password is being
     *        requested, may have one of the following values:
     *         - "add subkey": add subkey to the encrypted secret key
     *         - "add userid": add userid to the encrypted secret key
     *         - "sign": sign data
     *         - "decrypt": decrypt data using the encrypted secret key
     *         - "unlock": temporary unlock secret key (decrypting its fields), so it may be used
     *           later without need to decrypt
     *         - "protect": encrypt secret key fields
     *         - "unprotect": decrypt secret key fields, leaving those in a raw format
     *         - "decrypt (symmetric)": decrypt data, using the password
     *         - "encrypt (symmetric)": encrypt data, using the password
     * @param buf to which the callback should write the returned password, NULL terminated.
     * @param buf_len the size of buf
     *
     * @return true if a password was provided, false otherwise
     */
    static bool
    get_pass_provider(  rnp_ffi_t        ffi,
                        void            *app_ctx,
                        rnp_key_handle_t key,
                        const char      *pgp_context,
                        char             buf[],
                        size_t           buf_len)
    {
        qDebug() << "[UTPassphraseProvider] Call the getPassphrase";

        if (!UTPassphraseProvider::instance().m_window) {
            qWarning() << "[UTPassphraseProvider] Aborting : window is not set";
            return false;
        }

        if (!UTPassphraseProvider::instance().m_sem->tryAcquire(1, 500)) {
            qWarning() << "[UTPassphraseProvider] Aborting : Cannot acquire UTPassphraseProvider semaphore";
            return false;
        }

        UTPassphraseProvider::instance().m_passphrase = QString::Null();
        UTPassphraseProvider::instance().m_canceled = true;

        qDebug() << "[UTPassphraseProvider] Call the QML Dialog Passphrase Provider";
        QMetaObject::invokeMethod(
            UTPassphraseProvider::instance().m_window, "callPassphraseDialog",
            Q_ARG(QVariant, "useridHint"),      // TODO
            Q_ARG(QVariant, "description"),     // TODO
            Q_ARG(QVariant, "previousWasBad")   // TODO
        );

        qDebug() << "[UTPassphraseProvider] Waiting for response";

        QEventLoop loop;
        QObject::connect(&UTPassphraseProvider::instance(), &UTPassphraseProvider::unlockEventLoop, &loop, &QEventLoop::quit);
        loop.exec();

        qDebug() << "[UTPassphraseProvider] Prepare Returns";
        auto ret = false;
        if (!UTPassphraseProvider::instance().m_canceled) {
            strncpy(buf, UTPassphraseProvider::instance().m_passphrase.toLocal8Bit().data(), buf_len);
            ret = true;
        };

        qDebug() << "[UTPassphraseProvider] Clean Up";
        UTPassphraseProvider::instance().m_passphrase = QString::Null();
        UTPassphraseProvider::instance().m_canceled = true;
        UTPassphraseProvider::instance().m_sem->release(1);
        return ret;
    }

    /**
     * @brief Sets the window object that triggers the passphrase dialog.
     *
     * This method allows the passphrase provider to know which window should invoke the QML dialog.
     *
     * @param window The window object to set.
     */
    void setWindow(QObject* window)
    {
        this->m_window = window;
    }
};

#endif // UTPASSPHRASEPROVIDER_H
