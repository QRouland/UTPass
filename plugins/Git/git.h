#ifndef GIT_H
#define GIT_H

#include <QObject>
#include <QUrl>

class Git : public QObject
{
    Q_OBJECT

public:
    Git();
    ~Git() override = default;

};

#endif
