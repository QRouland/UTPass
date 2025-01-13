#ifndef GIT_H
#define GIT_H

#include <QUrl>
#include <QObject>
#include <QtCore/QDir>

#include "libgit.h"

class Git : public QObject
{
    Q_OBJECT

private:
    QDir clone_setup();
    bool move_to_destination(QString path, QDir tmp_dir);
    bool clone_tear_down(QDir tmp_dir);
    bool clone(QString url, QString path, mode_type mode);


public:
    Git() = default;
    ~Git() override = default;

    Q_INVOKABLE bool clone_http(QString url, QString path);
    Q_INVOKABLE bool clone_http_pass(QString url, QString path, QString pass);
    // Q_INVOKABLE bool clone_ssh_pass(QString url, QString path, QString pass);
    // Q_INVOKABLE bool clone_ssh_key(QString url, QString path, QString pub_key, QString priv_key, QString passphrase);
    // Q_INVOKABLE bool update(QUrl url, QString path);
};

#endif
