#ifndef GITJOB_H
#define GITJOB_H

#include <QThread>
extern "C" {
#include <git2.h>
}
#include <variant>

// Forward declarations for the different credential types.
struct HTTP { };
struct HTTPUserPass {
    QString pass; ///< Password for HTTP user authentication.
};
struct SSHPass { };
struct SSHKey { };

/**
 * @brief Variant type to represent various types of credentials.
 *
 * This type is used to store one of the following credential types:
 * - HTTP
 * - HTTPUserPass
 * - SSHPass
 * - SSHKey
 */
typedef std::variant<HTTP, HTTPUserPass, SSHPass, SSHKey> cred_type;

/**
 * @class GitJob
 * @brief A class that manages Git-related tasks using libgit2.
 *
 * The GitJob class is used abstraction class to perform Git operations, such as cloning repositories,
 * in a separate thread using libgit2 for interacting with Git repositories.
 */
class GitJob : public QThread
{
    Q_OBJECT

protected:
    cred_type m_cred; ///< The credentials used for Git operations.

    /**
     * @brief Callback function for handling Git credentials during cloning.
     *
     * This function is called by libgit2 to handle credentials for cloning a repository.
     * The callback is invoked when Git needs to authenticate with a remote repository.
     *
     * @param out Pointer to the credentials object that will be populated by the callback.
     * @param url The URL of the repository being cloned.
     * @param username_from_url The username extracted from the URL (if applicable).
     * @param allowed_types A bitmask of the allowed types of credentials that can be used.
     * @param payload User-defined data passed to the callback. This is typically the instance
     *        of the class providing the callback, or other user-defined data.
     *
     * @return A status code indicating success (0) or failure (non-zero).
     * @see git_cred
     * @see git_cred_type
     */
    static int credentialsCB(git_cred **out, const char *url, const char *username_from_url,
                             unsigned int allowed_types, void *payload);

public:
    /**
     * @brief Constructor for the GitJob class.
     *
     * Initializes the GitJob instance with the given credentials.
     *
     * @param cred The credentials to be used for the Git operation. This can be one of
     *        the following types: HTTP, HTTPUserPass, SSHPass, or SSHKey.
     */
    GitJob(cred_type cred);

    /**
     * @brief Destructor for the GitJob class.
     *
     * Cleans up any resources used by the GitJob.
     */
    ~GitJob();
};

#endif // GITJOB_H
