#include <QUrl>
#include <QtCore/QDir>
#include <QDebug>
#include <QStandardPaths>


#include "git.h"
#include "libgit.h"
#include "utils.h"


QDir Git::cloneSetup()
{
    QDir tmp_dir(QStandardPaths::writableLocation( QStandardPaths::CacheLocation).append("/clone"));

    tmp_dir.removeRecursively();
    qDebug() << "Temp dir path is " << tmp_dir.absolutePath();

    return tmp_dir;
}


bool Git::cloneTearDown(QDir tmp_dir)
{
    return tmp_dir.removeRecursively();
}

bool Git::moveToDestination(QString path, QDir tmp_dir)
{
    qDebug() << "Removing password_store " << path;
    QDir destination_dir(path);
    destination_dir.removeRecursively();

    qDebug() << "Moving cloned content to destination dir";
    QDir dir;
    qDebug() <<  tmp_dir.absolutePath() << " to " << destination_dir.absolutePath();
    return dir.rename(tmp_dir.absolutePath(), destination_dir.absolutePath()); // TODO Better error handling
}

bool Git::clone(QString url, QString path, mode_type mode) //, GitPlugin::RepoType type, QString pass)
{
    auto v =  overload {
        [](const Unset & x)  { return "Unset"; },
        [](const HTTP & x)   { return "HTTP"; },
        [](const HTTPAuth & x)   { return "HTTPAuth"; },
        [](const SSHAuth & x)   { return "SSHAuth"; },
        [](const SSHKey & x)   { return "SSHKey"; },
    };
    qInfo() << "Cloning " << url << " to destination " << path << " using " << std::visit(v, mode);

    LibGit::instance()->setMode(mode);
    auto tmp_dir = this->cloneSetup();

    qDebug() << "Cloning " << url << " to tmp dir " << tmp_dir.absolutePath();
    auto ret = LibGit::instance()->clone(url, tmp_dir.absolutePath()); // TODO Better error handling

    if (ret) {
        this->moveToDestination(path, tmp_dir);
    }

    this->cloneTearDown(tmp_dir);
    LibGit::instance()->setMode(Unset());

    return ret ;
}

bool Git::cloneHttp(QString url, QString path)
{
    HTTP mode = {};
    return this->clone(url, path, mode);
}

bool Git::cloneHttpPass(QString url, QString path, QString pass)
{
    HTTPAuth mode = { pass };
    return this->clone(url, path, mode);
}
