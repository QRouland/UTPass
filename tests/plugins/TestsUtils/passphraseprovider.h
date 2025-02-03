#ifndef UTPASSPHRASEPROVIDER_H
#define UTPASSPHRASEPROVIDER_H

#include <QObject>
#include <gpg-error.h>
extern "C" {
#include <rnp/rnp.h>
}

class TesTPassphraseProvider : public QObject
{
    Q_OBJECT
private:
    explicit TesTPassphraseProvider(QObject * parent = nullptr)
    {}

public:
    ~TesTPassphraseProvider() = default;

    static TesTPassphraseProvider& instance()
    {
        static TesTPassphraseProvider instance;
        return instance;
    }
    TesTPassphraseProvider(TesTPassphraseProvider const &) = delete;
    void operator=(TesTPassphraseProvider const &) = delete;


static bool
example_pass_provider(rnp_ffi_t        ffi,
                      void *           app_ctx,
                      rnp_key_handle_t key,
                      const char *     pgp_context,
                      char             buf[],
                      size_t           buf_len)
{        strncpy(buf, "utpasspassphrase", buf_len);
        return true;
}
};
#endif
