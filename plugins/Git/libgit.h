#ifndef LIBGIT_H
#define LIBGIT_H

#include <QObject>
#include <QUrl>
extern "C" {
#include <git2/transport.h>
}
#include <memory>

class LibGit
{
private:
    LibGit();
    static int credentials_cb(git_cred **out, const char *url, const char *username_from_url,
                            unsigned int allowed_types, void *payload);


public:
    ~LibGit();
    static std::shared_ptr<LibGit> instance()
    {
        static std::shared_ptr<LibGit> s{new LibGit};
        return s;
    }
    LibGit(LibGit const &) = delete;
    void operator=(LibGit const &) = delete;

    bool clone(QString url, QString path);
};

#endif
