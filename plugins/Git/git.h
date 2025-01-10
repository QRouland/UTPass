#ifndef GIT_H
#define GIT_H

#include <QUrl>
#include <QObject>


class Git : public QObject
{
    Q_OBJECT

public:
    Git() = default;
    ~Git() override = default;

    Q_INVOKABLE bool clone(QString url, QString path);
    // Q_INVOKABLE bool update(QUrl url, QString path);
};

#endif
