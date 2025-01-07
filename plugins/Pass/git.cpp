


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

bool Git::clone(QString url)
{
    return false;
}
