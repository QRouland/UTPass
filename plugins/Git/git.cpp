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
    m_sem(std::unique_ptr<QSemaphore>(new QSemaphore(1)))
{
    git_libgit2_init();
}

Git::~Git()
{
    git_libgit2_shutdown();
}


bool Git::clone(QString url, QString path, cred_type mode)
{
    if (!this->m_sem->tryAcquire(1, 500)) {
        qWarning() << "Can acquire git semaphore a command is already running ";
        return false;
    }
    auto v =  overload {
      [](const HTTP & x)        { return "HTTP"; },
        [](const HTTPUserPass & x)    { return "HTTPAuth"; },
      [](const SSHPass & x)     { return "SSHAuth"; },
      [](const SSHKey & x)      { return "SSHKey"; },
    };
    qDebug() << "Creating clone Job  " << url << " " << path << " " << std::visit(v, mode);
    CloneJob *clone_job = new CloneJob(url, path, mode);
    connect(clone_job, &CloneJob::resultReady, this, &Git::cloneResult);
    connect(clone_job, &CloneJob::finished, clone_job, &QObject::deleteLater);
    clone_job->start();
    return true;
}

bool Git::cloneHttp(QString url, QString path)
{
    qInfo() << "Call clone command Http " << url << " " << path;
    HTTP mode = {};
    return this->clone(url, path, mode);
}

bool Git::cloneHttpPass(QString url, QString path, QString pass)
{
    qInfo() << "Call clone command HttpPass " << url << " " << path;
    HTTPUserPass mode = { pass };
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
