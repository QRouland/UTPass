#include <QFile>
#include <QDir>
#include <QUrl>
#include <QtCore/QStandardPaths>
#include <quazip5/JlCompress.h>

#include "utils.h"


bool Utils::unzip(QUrl zip_url, QString dir_out_path)
{
    auto tmp_dir_path =  QStandardPaths::writableLocation(
                             QStandardPaths::CacheLocation).append("/unzip");

    QDir tmp_dir(tmp_dir_path);
    tmp_dir.removeRecursively();
    tmp_dir.mkpath(".");

    qDebug() << "Temp dir path is " << tmp_dir_path;
    auto status =  !JlCompress::extractDir(
                       zip_url.toLocalFile(),
                       tmp_dir_path
                   ).isEmpty();

    if (!status) {
        tmp_dir.removeRecursively();
        return false;
    }

    qDebug() << "Guessing if it should remove a single root folder";
    QStringList files_in_tmp_dir = tmp_dir.entryList(QDir::AllEntries | QDir::Hidden |
                                   QDir::NoDotAndDotDot);

    auto dir_import_path =
        files_in_tmp_dir.length() == 1 ?
        tmp_dir_path.append("/" + files_in_tmp_dir.first()) : tmp_dir_path;
    qDebug() << "Final imported tmp path dir is " << dir_import_path;

    qDebug() << "Removing destination";
    QDir dir_out(dir_out_path);
    dir_out.removeRecursively();

    qDebug() << "Moving zip content to destination";
    QDir dir;
    qDebug() <<  dir_import_path << " to " << dir_out_path;
    auto ret = dir.rename(dir_import_path, dir_out_path);
    tmp_dir.removeRecursively();;
    return ret;
}

bool Utils::rmFile(QUrl file_url)
{
    return QFile::remove(file_url.toLocalFile());
}

bool Utils::rmDir(QUrl dir_url)
{
    QDir dir(dir_url.toLocalFile());
    return dir.removeRecursively();
}

QString Utils::manifestPath(){
    auto path = QDir(QDir::currentPath()).filePath("manifest_.json");
    qDebug() << "Manifest path : " << path;
    return path;
}
