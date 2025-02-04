#include <QFile>
#include <QDir>
#include <QUrl>
#include <QtCore/QStandardPaths>
#include <quazip5/JlCompress.h>

#include "qdebug.h"
#include "unzipjob.h"

UnzipJob::UnzipJob(QUrl zip_url, QDir dir_out):
    m_zip_url(zip_url),
    m_dir_out(dir_out)
{
    this->setObjectName("UnzipJob");
}

void UnzipJob::run()
{
    auto tmp_dir_path =  QStandardPaths::writableLocation(
                             QStandardPaths::CacheLocation).append("/unzip");

    QDir tmp_dir(tmp_dir_path);
    tmp_dir.removeRecursively();
    tmp_dir.mkpath(".");

    qDebug() << "Temp dir path is " << tmp_dir_path;
    auto status =  !JlCompress::extractDir(
                       this->m_zip_url.toLocalFile(),
                       tmp_dir_path
                   ).isEmpty();

    if (!status) {
        tmp_dir.removeRecursively();
        emit resultReady(false);
        return;
    }

    qDebug() << "Guessing if it should remove a single root folder";
    QStringList files_in_tmp_dir = tmp_dir.entryList(QDir::AllEntries | QDir::Hidden |
                                   QDir::NoDotAndDotDot);

    auto dir_import_path =
        files_in_tmp_dir.length() == 1 ?
        tmp_dir_path.append("/" + files_in_tmp_dir.first()) : tmp_dir_path;
    qDebug() << "Final imported tmp path dir is " << dir_import_path;

    qDebug() << "Removing destination";
    this->m_dir_out.removeRecursively();

    qDebug() << "Moving zip content to destination";
    QDir dir;
    qDebug() <<  dir_import_path << " to " << this->m_dir_out;
    auto ret = dir.rename(dir_import_path, this->m_dir_out.absolutePath());
    tmp_dir.removeRecursively();;
    emit resultReady(!ret);
}
