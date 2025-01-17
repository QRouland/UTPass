#ifndef GIT_H
#define GIT_H

#include "jobs/gitjob.h"
#include <QUrl>
#include <QObject>
#include <QSemaphore>
#include <QtCore/QDir>

/**
 * @class Git
 * @brief A class that provides Git functionality for cloning and updating repositories.
 *
 * The `Git` class provides a set of methods to do Git operation on remote URLs.
 */
class Git : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Slot that handles the result of a cloning operation.
     *
     * This slot is connected to the result of the cloning operation and is triggered when the cloning
     * process finishes. It emits the appropriate signal based on whether the clone operation succeeded
     * or failed.
     *
     * @param err A boolean indicating whether an error occurred during cloning. `true` if the clone failed,
     *        `false` if it succeeded.
     */
    void cloneResult(const bool err);

signals:
    /**
     * @brief Signal emitted when the cloning operation succeeds.
     *
     * This signal is emitted when the Git repository is successfully cloned.
     */
    void cloneSucceed();

    /**
     * @brief Signal emitted when the cloning operation fails.
     *
     * This signal is emitted when an error occurs during the cloning operation.
     */
    void cloneFailed();

private:
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */

    /**
     * @brief Clones a repository from a specified URL.
     *
     * This private method initiates the cloning job. It sets up the repository cloning process based on
     * the specified URL, destination path, and cloning mode (HTTP or SSH).
     *
     * @param url The URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @param mode The cloning mode, such as HTTP or SSH (represented as `cred_type`).
     * @return `true` if the cloning process was successful, `false` otherwise.
     */
    bool clone(QString url, QString path, cred_type mode);

public:
    /**
     * @brief Constructor for the Git class.
     *
     * Initializes the `Git` class, setting up necessary resources such as the semaphore for concurrent operation management.
     */
    Git();

    /**
     * @brief Destructor for the Git class.
     *
     * Cleans up any resources used by the `Git` class, ensuring that no ongoing operations or resources are left hanging.
     */
    ~Git() override;

    /**
     * @brief Clones a repository over HTTP.
     *
     * This method clones a Git repository from the specified HTTP URL and saves it to the given destination path.
     * It is a wrapper around the private `clone()` method, specifying the HTTP cloning mode.
     *
     * @param url The HTTP URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @return `true` if the clone operation was successful, `false` otherwise.
     */
    Q_INVOKABLE bool cloneHttp(QString url, QString path);

    /**
     * @brief Clones a repository over HTTP with a password for authentication.
     *
     * This method clones a Git repository from the specified HTTP URL using the provided password for authentication,
     * and saves it to the given destination path.
     *
     * @param url The HTTP URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @param pass The password used for HTTP authentication.
     * @return `true` if the clone operation was successful, `false` otherwise.
     */
    Q_INVOKABLE bool cloneHttpPass(QString url, QString path, QString pass);

    // Future SSH support methods:
    // Q_INVOKABLE bool clone_ssh_pass(QString url, QString path, QString pass);
    // Q_INVOKABLE bool clone_ssh_key(QString url, QString path, QString pub_key, QString priv_key, QString passphrase);

    // Q_INVOKABLE bool update(QUrl url, QString path);
    // ....
};

#endif
