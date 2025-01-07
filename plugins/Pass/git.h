#ifndef GIT_H
#define GIT_H

#include <QObject>
#include <QUrl>
#include <memory>

class Git
{
private:
    Git();
public:

    ~Git();

    static std::shared_ptr<Git> instance()
    {
        static std::shared_ptr<Git> s{new Git};
        return s;
    }
    Git(Git const &) = delete;
    void operator=(Git const &) = delete;

    bool clone(QString url, QString path);
};

#endif
