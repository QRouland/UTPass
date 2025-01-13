#ifndef LIBGIT_H
#define LIBGIT_H

#include <QObject>
#include <QUrl>
#include <git2/clone.h>
extern "C" {
#include <git2/transport.h>
}
#include <memory>
#include <variant>

struct Unset { };
struct HTTP { };
struct HTTPAuth { QString pass; };
struct SSHAuth { };
struct SSHKey { };
typedef std::variant<Unset, HTTP, HTTPAuth, SSHAuth, SSHKey> mode_type;

class LibGit
{
private:
    LibGit();

    mode_type mode;

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
    void set_mode(mode_type type);
};

#endif
