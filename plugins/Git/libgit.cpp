#include <QUrl>
#include <QDebug>
extern "C" {
#include <git2.h>
}

#include "libgit.h"



LibGit::LibGit()
{
    git_libgit2_init();
}

LibGit::~LibGit()
{
    git_libgit2_shutdown();
}

int LibGit::credentials_cb(git_cred **out, const char *url, const char *username_from_url,
                           unsigned int allowed_types, void *payload)
{
    int error;
    const char *user, *pass;

    /*
     * Ask the user via the UI. On error, store the information and return GIT_EUSER which will be
     * bubbled up to the code performing the fetch or push. Using GIT_EUSER allows the application
     * to know it was an error from the application instead of libgit2.
     */
    // if ((error = ask_user(&user, &pass, url, username_from_url, allowed_types)) < 0) {
    //     store_error(error);
    //     return GIT_EUSER;
    // }
    // user = "user";
    // pass = "pass";
    // return git_cred_userpass_plaintext_new(out, user, pass);
    return GIT_EUSER;
}

bool LibGit::clone(QString url, QString path)
{
    git_repository *repo = NULL;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    opts.fetch_opts.callbacks.credentials = *credentials_cb;
    int ret = git_clone(&repo, url.toLocal8Bit().data(), path.toLocal8Bit().data(), &opts);
    if (repo) {
        git_repository_free(repo);
    }
    return ret == 0; // TODO Clean error handling to return specifics errors for the ui
}
