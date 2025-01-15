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

/**
 * @class UTPassphraseProvider
 * @brief A passphrase provider for GPG operations that interacts with a QML dialog.
 *
 * This class implements the `PassphraseProvider` interface from GPGME and is responsible for
 * obtaining passphrases for GPG operations. It does so by triggering a QML dialog to prompt the user
 * for their passphrase. The response is then handled asynchronously, and the passphrase is returned to
 * the calling GPGME function.
 */
class UTPassphraseProvider : public QObject, public PassphraseProvider
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
            gpgrt_asprintf(&m_passphrase, "%s", passphrase.toUtf8().constData());
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
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing access. */
    char *m_passphrase; /**< The passphrase provided by the user. */
    bool m_canceled; /**< Flag indicating whether the passphrase operation was canceled. */
    QObject *m_window; /**< The window object that triggers the QML dialog. */

public:
    /**
     * @brief Constructs a UTPassphraseProvider.
     *
     * Initializes the semaphore, passphrase, and canceled flag. Sets the window object that will
     * trigger the passphrase dialog.
     *
     * @param window The QObject representing the window that interacts with QML.
     */
    UTPassphraseProvider(QObject* window)
        : m_sem(std::make_unique<QSemaphore>(1)),
          m_passphrase(nullptr),
          m_canceled(false),
          m_window(window)
    {
        qDebug() << "Initialize UTPassphraseProvider";
    }

    /**
     * @brief Implements the PassphraseProvider's `getPassphrase` method.
     *
     * This method is called by GPGME to retrieve the passphrase needed for GPG operations. It triggers
     * a QML dialog for the user to input their passphrase. The method waits for the response and returns
     * the passphrase if successful, or null if canceled.
     *
     * @param useridHint A hint for the user ID to which the passphrase corresponds.
     * @param description A description of the passphrase request.
     * @param previousWasBad Flag indicating whether the previous passphrase attempt was incorrect.
     * @param canceled Reference to a boolean flag that will be set if the operation is canceled.
     *
     * @return The passphrase as a `char *` or `nullptr` if canceled.
     */
    char *getPassphrase(const char *useridHint,
                        const char *description,
                        bool previousWasBad,
                        bool &canceled) Q_DECL_OVERRIDE {
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
