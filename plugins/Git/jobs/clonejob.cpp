#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QObject>
#include <memory>
#include <type_traits>
extern "C" {
#include <git2.h>
}

#include "clonejob.h"
#include "../error.h"

CloneJob::CloneJob(QString url, QString path, cred_type cred):
    GitJob(cred),
    m_url(url),
    m_path(path)
{
    this->setObjectName("CloneJob");
}

void CloneJob::run()
{
    auto tmp_dir = this->cloneSetup();
    const auto [errorCode, errorMessage] = this->clone(m_url, tmp_dir.absolutePath(), this->m_cred, this->credentialsCB);

    if (errorCode == GitCloneErrorCode::Successful) {
        this->moveToDestination(tmp_dir, this->m_path);
    }

    this->cloneCleanUp(tmp_dir);
    emit resultReady(code_err(errorCode), errorMessage);
}

QDir CloneJob::cloneSetup()
{
    QDir tmp_dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation).append("/clone"));

    tmp_dir.removeRecursively();  // Clean the directory before use
    qDebug() << "[CloneJob] Temp dir path is" << tmp_dir.absolutePath();

    return tmp_dir;
}

bool CloneJob::cloneCleanUp(QDir tmp_dir)
{
    if (!tmp_dir.removeRecursively()) {
        qWarning() << "[CloneJob] Failed to clean up temporary directory:" << tmp_dir.absolutePath();
        return false;
    }
    return true;
}

bool CloneJob::moveToDestination(QDir tmp_dir, const QString& path)
{
    qDebug() << "[CloneJob] Removing existing destination:" << path;

    QDir destination_dir(path);
    destination_dir.removeRecursively();  // Clean the destination directory

    qDebug() << "[CloneJob] Moving cloned content to destination dir";

    if (!QDir().rename(tmp_dir.absolutePath(), destination_dir.absolutePath())) {
        qWarning() << "[CloneJob] Failed to move directory from" << tmp_dir.absolutePath() << "to" <<
                   destination_dir.absolutePath();
        return false;
    }

    return true;
}

const QPair<GitCloneErrorCode, QString> CloneJob::clone(QString url, QString path, cred_type cred,
        git_cred_acquire_cb cb)
{
    git_repository *repo = nullptr;  // Use nullptr for type safety
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    PayloadCB payload(false, cred);

    opts.fetch_opts.callbacks.credentials = cb;
    opts.fetch_opts.callbacks.payload = &payload;

    int ret = git_clone(&repo, url.toLocal8Bit().data(), path.toLocal8Bit().data(), &opts);

    // Map the application specific cb errors if any
    if (ret == GIT_EUSER) {
        if (payload.err == ErrorCodeCB::NoUsername)
            return {GitCloneErrorCode::NoUsername, "no username provided in URL"};
        if (payload.err == ErrorCodeCB::InvalidCreds)
            return {GitCloneErrorCode::AuthentificationError, "authentification error"};
        if (payload.err == ErrorCodeCB::UrlTypeDoNotMatchCreds)
            return {GitCloneErrorCode::UrlTypeDoNotMatchCreds, "invalid creds types for provided url"};
        return {GitCloneErrorCode::UnexpectedError, "unexcepted error occured"};
    }

    const git_error* err = git_error_last();  // Retrieve the last error git error

    // Log error details for debugging
    if (err) {
        qDebug() << "[CloneJob] Error class:" << err->klass;
        qDebug() << "[CloneJob] Error message:" << err->message;
    }

    // Check if the repository was successfully created and free it
    if (repo) {
        git_repository_free(repo);
    }

    // Return the error code mapped from git_error_last
    return { gitErrorToGitCloneErrorCode(err), err ? QString::fromUtf8(err->message) : "success"};
}
