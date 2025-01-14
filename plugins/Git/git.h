#ifndef GIT_H
#define GIT_H

#include <QUrl>
#include <QObject>
#include <QtCore/QDir>

#include "libgit.h"

/**
 * @brief The Git class is class that provide Git functionnly to clone and update a repo.
 */
class Git : public QObject
{
    Q_OBJECT

private:
    QDir cloneSetup();
    bool moveToDestination(QString path, QDir tmp_dir);
    bool cloneTearDown(QDir tmp_dir);
    bool clone(QString url, QString path, mode_type mode);


public:
    Git() = default;
    ~Git() override = default;

    Q_INVOKABLE bool cloneHttp(QString url, QString path);
    Q_INVOKABLE bool cloneHttpPass(QString url, QString path, QString pass);
    // Q_INVOKABLE bool clone_ssh_pass(QString url, QString path, QString pass);
    // Q_INVOKABLE bool clone_ssh_key(QString url, QString path, QString pub_key, QString priv_key, QString passphrase);
    // Q_INVOKABLE bool update(QUrl url, QString path);
};

#endif
