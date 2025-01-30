#ifndef UTPASSPHRASEPROVIDER_H
#define UTPASSPHRASEPROVIDER_H

#include <QObject>
#include <gpg-error.h>
#include <gpgme++/interfaces/passphraseprovider.h>

class TesTPassphraseProvider : public QObject, public GpgME::PassphraseProvider
{
    Q_OBJECT

public:
    char *getPassphrase(const char *useridHint,
                        const char *description,
                        bool previousWasBad,
                        bool &canceled) override
    {

        char *ret;
        gpgrt_asprintf(&ret, "%s", "utpasspassphrase");
        return ret;
    };
};

#endif
