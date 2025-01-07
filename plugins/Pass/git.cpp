


#include <QDebug>
#include <QUrl>
extern "C" {
#include <git2.h>
}

#include "git.h"



Git::Git()
{
    git_libgit2_init();
}

Git::~Git() {
    git_libgit2_shutdown();
}

bool Git::clone(QString url, QString path) {
    git_repository *repo = NULL;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

    int ret = git_clone(&repo, url.toLocal8Bit().data(), path.toLocal8Bit().data(), &opts);
    if (repo) {
        git_repository_free(repo);
    }
    return ret == 0; // TODO better error handling to return specifics errors for the ui
}
