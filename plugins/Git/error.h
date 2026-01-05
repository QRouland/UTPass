// error.h
#ifndef ERROR_H
#define ERROR_H

#include <string>
extern "C" {
#include <git2.h>
}

// Enum for Git-specific errors (e.g., from git_clone)
enum class GitCloneErrorCode {
    Successful = 0,
    UnexpectedError,  ///< Unknown or unexpected error
    InvalidUrl,           ///< Malformed URL error
    NoUsername,           ///< Missing username error
    AuthentificationError,   ///< Authentification error
    UrlTypeDoNotMatchCreds,
};

/**
* Convert a git_error to a GitCloneErrorCode
* @param error A pointer to the git_error structure
* @return Corresponding GitCloneErrorCode integer value
*/
inline GitCloneErrorCode gitErrorToGitCloneErrorCode(const git_error* error) {
    if (error == nullptr) {
        return GitCloneErrorCode::Successful;  ///< Default error if null
    }

    if (error->message != nullptr) {
        if (std::string(error->message) == "malformed URL") {
            return GitCloneErrorCode::InvalidUrl;  ///< Invalid URL error
        }
        if (std::string(error->message) == "remote requested authentication but did not negotiate mechanisms") {
            return GitCloneErrorCode::AuthentificationError;  ///< Invalid URL error
        }
    }

    return GitCloneErrorCode::UnexpectedError;  ///< Default to UnexpectedError
}

/**
 * Maps the given GitCloneErrorCode to its corresponding integer.
 *
 * @param error The GitCloneErrorCode value to convert.
 * @return Corresponding GitCloneErrorCode integer value.
 */
constexpr int code_err(GitCloneErrorCode err)
{
    return static_cast<int>(err);
}


#endif // ERROR_H
