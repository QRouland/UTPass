#ifndef CLONEJOB_H
#define CLONEJOB_H

#include <QDir>
extern "C" {
#include <git2.h>
}
#include "gitjob.h"

/**
 * @class CloneJob
 * @brief A class to handle cloning Git repositories in a separate thread.
 *
 */
class CloneJob : public GitJob
{
    Q_OBJECT

    /**
     * @brief The main function that performs the cloning operation.
     *
     * Handles the process of cloning a repository from the specified URL
     * to the target path.
     */
    void run() override;

signals:
    /**
     * @brief Signal emitted when the cloning operation is complete.
     *
     * This signal is emitted once the cloning operation finishes.
     *
     * @param err A boolean indicating whether an error occurred during cloning.
     *        `true` if an error occurred, `false` if the clone was successful.
     */
    void resultReady(const bool err);

private:
    QString m_url; ///< The URL of the Git repository to clone.
    QString m_path; ///< The destination path for the cloned repository.

    /**
     * @brief Prepares the temporary directory for cloning.
     *
     * This method sets up the required directory structure for cloning a repository.
     *
     * @return A `QDir` object representing the prepared temporary directory.
     */
    static QDir cloneSetup();

    /**
     * @brief Moves the cloned repository to the specified destination.
     *
     * After the repository has been cloned into a temporary directory, this method
     * moves it to the final destination specified by the user.
     *
     * @param path The destination path to move the repository to.
     * @param tmp_dir The temporary directory where the repository was cloned.
     * @return `true` if the move was successful, `false` otherwise.
     */
    static bool moveToDestination(QDir tmp_dir, QString path);

    /**
     * @brief Tears down the temporary directory after cloning.
     *
     * This method is called to clean up the temporary directory after the repository
     * has been cloned and moved to the final destination. It removes the temporary
     * directory and all its contents.
     *
     * @param tmp_dir The temporary directory to tear down.
     * @return `true` if the teardown was successful, `false` otherwise.
     */
    static bool cloneCleanUp(QDir tmp_dir);

    /**
     * @brief Clones a repository from a specified URL.
     *
     * This method handles the actual cloning process by setting up a temporary
     * directory, cloning the repository into it, and then moving it to the final
     * destination. It uses the provided credentials to authenticate the cloning
     * operation.
     *
     * @param url The URL of the Git repository to clone.
     * @param path The destination path for the cloned repository.
     * @param cred The credentials to use for the cloning operation.
     * @param cb The callback function for acquiring credentials during cloning.
     * @return `true` if the cloning process was successful, `false` otherwise.
     */
    static bool clone(QString url, QString path, cred_type cred, git_cred_acquire_cb cb);

public:
    /**
     * @brief Constructor for the CloneJob class.
     *
     * Initializes the CloneJob with the specified repository URL, destination path,
     * and credentials.
     *
     * @param url The URL of the Git repository to clone.
     * @param path The destination path where the repository will be cloned.
     * @param cred The credentials to be used for the cloning process.
     */
    CloneJob(QString url, QString path, cred_type cred);
};

#endif // CLONEJOB_H
