#include <QUrl>
#include <QDebug>

#include <type_traits>
extern "C" {
#include <git2.h>
}

#include "libgit.h"
#include "utils.h"



LibGit::LibGit()
{
    git_libgit2_init();
}

LibGit::~LibGit()
{
    git_libgit2_shutdown();
}

void LibGit::setMode(mode_type type)
{
    this->mode = type;
}

int LibGit::credentialsCB(git_cred **out, const char *url, const char *username_from_url,
                           unsigned int allowed_types, void *payload)
{
    // TODO : More precise Error Handling for UI
    auto instance = LibGit::instance();
    auto v =  overload {
        [](const Unset & x)
        {
            qDebug() << "credentials_cb : Unset ";
            qWarning() << "credentials_cb : callback should never be call for Unset ";
            return (int) GIT_EUSER;
        },
        [](const HTTP & x)
        {
            qDebug() << "credentials_cb : HTTP ";
            qWarning() << "credentials_cb : callback should never be call for HTTP ";
            return (int) GIT_EUSER;
        },
        [&out, &username_from_url](const HTTPAuth & x)
        {
            qDebug() << "credentials_cb : HTTPAuth ";
            if (!username_from_url) {
                qWarning() << "credentials_cb : no username provided ";
                return (int) GIT_EUSER;
            }
            return git_cred_userpass_plaintext_new(out, username_from_url, x.pass.toLocal8Bit().constData());
        },
        [&](const SSHAuth & x)
        {
            qWarning() << "credentials_cb : SSHAuth to be implemented ";
            return (int) GIT_EUSER;
        }, // TODO
        [&](const SSHKey & x)
        {
            qWarning() << "credentials_cb : SSHKey to be implemented ";
            return (int) GIT_EUSER;
        }  // TODO
    };
    return std::visit(v, instance->mode);
}


bool LibGit::clone(QString url, QString path)
{
    git_repository *repo = NULL;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

    opts.fetch_opts.callbacks.credentials = *credentialsCB;

    int ret = git_clone(&repo, url.toLocal8Bit().data(), path.toLocal8Bit().data(), &opts);
    if (ret != 0) {
        qDebug() << git_error_last()->message;
    }
    if (repo) {
        git_repository_free(repo);
    }
    return ret == 0; // TODO Clean error handling to return specifics errors for the ui

    return ret;
}

