#include <QDebug>

#include "gitjob.h"
#include "../utils.h"

extern "C" {
#include <git2.h>
}

GitJob::GitJob(cred_type cred) :
    m_cred(cred)
{
    git_libgit2_init();
}

GitJob::~GitJob()
{
    git_libgit2_shutdown();
}

int GitJob::credentialsCB(git_cred **out, const char *url, const char *username_from_url,
                          unsigned int allowed_types, void *payload)
{
    cred_type *cred = (cred_type *)payload;
    auto v =  overload {
        [](const HTTP & x)
        {
            qDebug() << "credentialsCB : HTTP ";
            qWarning() << "credentialsCB : callback should never be call for HTTP ";
            return (int) GIT_EUSER;
        },
        [&out, &username_from_url](const HTTPUserPass & x)
        {
            qDebug() << "credentialsCB : HTTPUserPass ";
            if (!username_from_url) {
                qWarning() << "credentials_cb : no username provided ";
                return (int) GIT_EUSER;
            }
            return git_cred_userpass_plaintext_new(out, username_from_url, x.pass.toLocal8Bit().constData());
        },
        [](const SSHPass & x)
        {
            qWarning() << "credentials_cb : SSHAuth to be implemented ";
            return (int) GIT_EUSER;
        }, // TODO
        [](const SSHKey & x)
        {
            qWarning() << "credentials_cb : SSHKey to be implemented ";
            return (int) GIT_EUSER;
        }  // TODO
    };
    return std::visit(v, *cred);
}
