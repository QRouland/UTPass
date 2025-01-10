#include <QUrl>
#include <QtCore/QDir>
#include <QDebug>

#include "git.h"
#include "libgit.h"


Git::Git()
{}

bool Git::clone(QString url, QString path)
{
    qInfo() << "Cloning " << url << "password_store to " << path;
    QDir dir(path);
    dir.removeRecursively(); // TODO see if we delete only after sucessfull clone / Will be change anyway when will add update etc..
    return LibGit::instance()->clone(url, path);
}
