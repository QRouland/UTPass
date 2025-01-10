#include <QUrl>
#include <QtCore/QDir>
#include <QDebug>
#include <QStandardPaths>

#include "git.h"
#include "libgit.h"


bool Git::clone(QString url, QString destination_dir_path)
{
    qInfo() << "Cloning " << url << " to destination " << destination_dir_path;
    QDir tmp_dir(QStandardPaths::writableLocation(
                     QStandardPaths::CacheLocation).append("/clone"));
    tmp_dir.removeRecursively();
    tmp_dir.mkpath(".");
    qDebug() << "Temp dir path is " << tmp_dir.absolutePath();

    qDebug() << "Cloning " << url << " to tmp dir " << tmp_dir.absolutePath();
    auto ret = LibGit::instance()->clone(url, tmp_dir.absolutePath()); // TODO Better error handling

    if(ret) {
        qDebug() << "Removing password_store " << destination_dir_path;
        QDir destination_dir(destination_dir_path);
        destination_dir.removeRecursively();

        qDebug() << "Moving cloned content to destination dir";
        QDir dir;
        qDebug() <<  tmp_dir.absolutePath() << " to " << destination_dir.absolutePath();
        ret = dir.rename(tmp_dir.absolutePath(), destination_dir.absolutePath()); // TODO Better error handling
    }
    //tmp_dir.removeRecursively();
    return ret ;
}
