#ifndef GIT_H
#define GIT_H

#include "error.h"
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
    Q_PROPERTY(QString privKey READ pubKeyPath)
    Q_PROPERTY(QString pubKey  READ privKeyPath)

private slots:
    /**
     * @brief Slot that handles the result of a cloning operation.
     *
     * This slot is connected to the result of the cloning operation and is triggered when the cloning
     * process finishes. It emits the appropriate signal based on whether the clone operation succeeded
     * or failed.
     *
     * @param err A err_code indicating whether an error occurred during cloning.
     * @param err An error message.
     */
    void cloneResult(const int err_code, const QString message);

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
     * @param err_code The error code
     * @param msg The deffautl error message from libgit
     * This signal is emitted when an error occurs during the cloning operation.
     */
    void cloneFailed(int err_code, QString msg);

private:
    std::unique_ptr<QSemaphore> m_sem; /**< Semaphore for managing concurrent operations. */
    QDir m_ssh_homedir; /**< Directory that contains the SSH keys (public and private). */

    /**
     * @brief Clones a repository from a specified URL.
     *
     * This private method initiates the cloning job. It sets up the repository cloning process based on
     * the specified URL, destination path, and cloning mode (HTTP, HTTP_AUTH or SSH).
     *
     * @param url The URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @param mode The cloning mode, such as HTTP or SSH (represented as `cred_type`).
     * @return `true` if the cloning process was successful, `false` otherwise.
     */
    bool clone(QString url, QString path, cred_type mode);

protected:
    /**
     * @brief Get the path to the public keyring.
     *
     * @return The file path to the public key.
     */
    QString pubKeyPath()
    {
        return this->m_ssh_homedir.filePath("id_rsa.pub");
    }

    /**
     * @brief Get the path to the secret keyring.
     *
     * @return The file path to the private key.
     */
    QString privKeyPath()
    {
        return this->m_ssh_homedir.filePath("id_rsa");
    }

public:
    /**
     * @brief Constructor for the Git class.
     *
     * Initializes the `Git` class.
     */
    Git();

    /**
     * @brief Destructor for the Git class.
     *
     * Cleans up any resources used by the `Git` class.
     */
    ~Git() override;


    Q_INVOKABLE bool importSshKey(QUrl source_path, bool is_private);

    /**
     * @brief Clones a repository over HTTP.
     *
     * This method clones a Git repository from the specified HTTP URL and saves it to the given destination path.
     *
     * @param url The HTTP URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @return `true` if the clone operation was successfully started, `false` otherwise.
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
     * @return `true` if the clone job operation was successfully started, `false` otherwise.
     */
    Q_INVOKABLE bool cloneHttpPass(QString url, QString path, QString pass);

    /**
     * @brief Clones a repository over SSH with a key for authentication.
     *
     * This method clones a Git repository from the specified ssh URL using the provided password for authentication,
     * and saves it to the given destination path.
     *
     * @param url The HTTP URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @param passphrase The passphrase used for SSH authentication.
     * @return `true` if the clone job operation was successfully started, `false` otherwise.
     */
    Q_INVOKABLE bool cloneSshKey(QString url, QString path, QString passphrase);

    // Q_INVOKABLE bool update(QUrl url, QString path);
    // ....
};

#endif
