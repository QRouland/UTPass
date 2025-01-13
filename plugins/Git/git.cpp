#include <QUrl>
#include <QtCore/QDir>
#include <QDebug>
#include <QStandardPaths>


#include "git.h"
#include "libgit.h"


template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;


QDir Git::clone_setup()
{
    QDir tmp_dir(QStandardPaths::writableLocation( QStandardPaths::CacheLocation).append("/clone"));

    tmp_dir.removeRecursively();
    qDebug() << "Temp dir path is " << tmp_dir.absolutePath();

    return tmp_dir;
}


bool Git::clone_tear_down(QDir tmp_dir)
{
    tmp_dir.removeRecursively();
}

bool Git::move_to_destination(QString path, QDir tmp_dir)
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
        [](const Unset& x)  { return "Unset"; },
        [](const HTTP& x)   { return "Unset"; },
        [](const HTTPAuth& x)   { return "HTTPAuth"; },
        [](const SSHAuth& x)   { return "SSHAuth"; },
        [](const SSHKey& x)   { return "SSHKey"; },
    };
    qInfo() << "Cloning " << url << " to destination " << path << " using " << std::visit(v, mode);

    LibGit::instance()->set_mode(mode);
    auto tmp_dir = this->clone_setup();

    qDebug() << "Cloning " << url << " to tmp dir " << tmp_dir.absolutePath();
    auto ret = LibGit::instance()->clone(url, tmp_dir.absolutePath()); // TODO Better error handling

    if (ret) { this->move_to_destination(path, tmp_dir);}

    tmp_dir.removeRecursively();
    LibGit::instance()->set_mode(Unset());

    return ret ;
}

bool Git::clone_http(QString url, QString path) //, GitPlugin::RepoType type, QString pass)
{
    HTTP mode = {};
    return this->clone(url, path, mode);
}

bool Git::clone_http_pass(QString url, QString path, QString pass) {
    HTTPAuth mode = { pass };
    return this->clone(url, path, mode);
}
