#include <QStandardPaths>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QObject>
#include <type_traits>
extern "C" {
#include <git2.h>
}

#include "clonejob.h"

CloneJob::CloneJob(QString url, QString path, cred_type cred):
    GitJob(cred),
    m_url(url),
    m_path(path)
{
    this->setObjectName("CloneJob");
}


void CloneJob::run()
{
    auto tmp_dir = this->cloneSetup();
    auto err = this->clone(this->m_url,  tmp_dir.absolutePath(), this->m_cred, this->credentialsCB);
    if (err) {
        this->moveToDestination(tmp_dir, this->m_path);
    }
    this->cloneTearDown(tmp_dir);

    emit resultReady(err); // TODO Clean error handling to return specifics errors for the ui
}


QDir CloneJob::cloneSetup()
{
    QDir tmp_dir(QStandardPaths::writableLocation( QStandardPaths::CacheLocation).append("/clone"));

    tmp_dir.removeRecursively();
    qDebug() << "Temp dir path is " << tmp_dir.absolutePath();

    return tmp_dir;
}


bool CloneJob::cloneTearDown(QDir tmp_dir)
{
    return tmp_dir.removeRecursively();
}

bool CloneJob::moveToDestination(QDir tmp_dir, QString path)
{
    qDebug() << "Removing password_store " << path;
    QDir destination_dir(path);
    destination_dir.removeRecursively();

    qDebug() << "Moving cloned content to destination dir";
    QDir dir;
    qDebug() <<  tmp_dir.absolutePath() << " to " << destination_dir.absolutePath();
    return dir.rename(tmp_dir.absolutePath(), destination_dir.absolutePath()); // TODO Better error handling
}

bool CloneJob::clone(QString url, QString path, cred_type cred, git_cred_acquire_cb cb)
{
    git_repository *repo = NULL;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

    opts.fetch_opts.callbacks.credentials = cb;
    opts.fetch_opts.callbacks.payload = &cred;

    int ret = git_clone(&repo, url.toLocal8Bit().data(), path.toLocal8Bit().data(), &opts);
    if (ret != 0) {
        qDebug() << git_error_last()->message;
    }
    if (repo) {
        git_repository_free(repo);
    }// TODO Better error handling
    return ret != 0;
}

