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
    UNUSED(url);
    cred_type *cred = (cred_type *)payload;

    if (!username_from_url) {
        qWarning() << "[GitJob] credentials_cb : no username provided ";
        return (int) GIT_EUSER;
    }

    auto v =  overload {
        [](const HTTP & x)
        {
            UNUSED(x);
            qDebug() << "[GitJob] credentialsCB : None ";
            qWarning() << "[GitJob] credentialsCB : callback should never be call for None ";
            return (int) GIT_EUSER;
        },
        [allowed_types, &out, &username_from_url](const HTTPUserPass & x)
        {
            qDebug() << "[GitJob] credentialsCB : HTTPUserPass ";
            if (!(allowed_types & GIT_CREDTYPE_USERPASS_PLAINTEXT)) {
                qWarning() << "[GitJob] credentials_cb : allowed_types is invalid for HTTPUserPass ";
                return (int) GIT_EUSER;
            }
            return git_cred_userpass_plaintext_new(out, username_from_url, x.pass.toLocal8Bit().constData());
        },
        [allowed_types, &out, &username_from_url](const SSHKey & x)
        {
            qDebug() << "[GitJob] credentialsCB : SSHKey ";
            if (!(allowed_types & GIT_CREDTYPE_SSH_KEY)) {
                qWarning() << "[GitJob] credentials_cb : allowed_types is invalid for HTTPUserPass ";
                return (int) GIT_EUSER;
            }
            return git_cred_ssh_key_new(out, username_from_url, x.pub_key.toLocal8Bit().constData(),
                                        x.priv_key.toLocal8Bit().constData(), x.passphrase.toLocal8Bit().constData());
        }
    };
    auto error = std::visit(v, *cred);
    return error;
}
