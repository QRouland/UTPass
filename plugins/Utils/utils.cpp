#include <QDir>
#include <QSemaphore>

#include "jobs/unzipjob.h"
#include "utils.h"



Utils::Utils():
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1)))
{}

bool Utils::unzip(QUrl zip_url, QString dir_out_path)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        return false;
    }
    qInfo() << "[Utils] Unzip path " << zip_url << " to " << dir_out_path;
    auto job = new UnzipJob(zip_url, QDir(dir_out_path));
    connect(job, &UnzipJob::resultReady, this, &Utils::unzipResult);
    connect(job, &UnzipJob::finished, job, &QObject::deleteLater);
    job->start();
    return true;
}

void Utils::unzipResult(bool err)
{

    qDebug() << "[Utils] Unzip Result";
    if (err) {
        qInfo() << "[Utils] Unzip Failed";
        emit unzipFailed();

    } else {
        qInfo() << "[Utils] Unzip Succeed";
        emit unzipSucceed();
    }
    this->m_sem->release(1);
}


QString Utils::manifestPath()
{
    auto path = QDir(QDir::currentPath()).filePath("manifest_.json");
    qInfo() << "[Utils] Manifest path : " << path;
    return path;
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

bool Utils::fileExists(QUrl path)
{
    QString p = path.toLocalFile();
    auto ret = QFileInfo::exists(p) && QFileInfo(p).isFile();
    qDebug() << "[Utils]" << path << " existing file :" << ret;
    return ret;
}

