#include <QUrl>
#include <QtCore/QDir>
#include <QDebug>
#include <QStandardPaths>
extern "C" {
#include <git2.h>
}

#include "git.h"
#include "utils.h"
#include "jobs/clonejob.h"
#include "jobs/gitjob.h"

Git::Git():
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1))),
    m_ssh_homedir (QStandardPaths::writableLocation(
                      QStandardPaths::AppDataLocation).append("/.ssh"))
{
    qDebug() << "[Git] SSH Home is " << m_ssh_homedir.absolutePath();
    QDir m_ssh_homedir(this->m_ssh_homedir);
    if (!m_ssh_homedir.exists()) {
        m_ssh_homedir.mkpath(".");
    }
    git_libgit2_init();
}

Git::~Git()
{
    git_libgit2_shutdown();
}


bool Git::clone(QString url, QString path, cred_type mode)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        qWarning() << "[Git] Can acquire git semaphore a command is already running ";
        return false;
    }
    auto v =  overload {
        [](const HTTP & x)        { UNUSED(x); return "HTTP"; },
        [](const HTTPUserPass & x)    { UNUSED(x); return "HTTPAuth"; },
        [](const SSHKey & x)      { UNUSED(x); return "SSHKey"; },
    };
    qDebug() << "[Git] Creating clone Job  " << url << " " << path << " " << std::visit(v, mode);
    CloneJob *clone_job = new CloneJob(url, path, mode);
    connect(clone_job, &CloneJob::resultReady, this, &Git::cloneResult);
    connect(clone_job, &CloneJob::finished, clone_job, &QObject::deleteLater);
    clone_job->start();
    return true;
}

bool Git::cloneHttp(QString url, QString path)
{
    qInfo() << "[Git] Call clone command Http " << url << " " << path;
    HTTP mode = {};
    return this->clone(url, path, mode);
}

bool Git::cloneHttpPass(QString url, QString path, QString pass)
{
    qInfo() << "[Git] Call clone command HttpPass " << url << " " << path;
    HTTPUserPass mode = { pass };
    return this->clone(url, path, mode);
}

bool Git::cloneSshKey(QString url, QString path, QString passphrase)
{
    qInfo() << "[Git] Call clone command SshKey " << url << " " << path;

    SSHKey mode = { this->pubKeyPath(), this->privKeyPath(), passphrase };
    return this->clone(url, path, mode);
}

void Git::cloneResult(const bool err)
{

    if (err) {
        emit cloneFailed(); // TODO error message
    } else {
        emit cloneSucceed();
    }
    this->m_sem->release();
}

bool Git::importSshKey(QUrl source_path, bool is_private){
    auto destination_path = is_private ? this->privKeyPath() : this->pubKeyPath();

    QFile source_file(source_path.toLocalFile());

    if (!source_file.exists()) {
        qWarning() << "[Git] Source file does not exist.";
        return false;
    }

    QDir target_dir = QFileInfo(destination_path).absoluteDir();
    if (!target_dir.exists()) {
        if (!target_dir.mkpath(".")) {
            qWarning() << "[Git] Failed to create target directory.";
            return false;
        }
    }
    return source_file.copy(destination_path);
}
