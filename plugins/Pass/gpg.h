#ifndef GPG_H
#define GPG_H

#include <memory>
#include "gpgme++/key.h"

class Gpg
{
private:
    Gpg();
    QDir m_gpghome;

public:
    ~Gpg();
    static std::shared_ptr<Gpg> instance()
    {
        static std::shared_ptr<Gpg> s{new Gpg};
        return s;
    }
    Gpg(Gpg const &) = delete;
    void operator=(Gpg const &) = delete;


    QString decrypt(QByteArray plainText);
    QString decryptFromFile(QString path);
    QByteArray encrypt(QString str, QString uid, bool ascii_armor = true, bool text_mode = true);
    bool encryptToFile(QString str, QString path, const QString uid, bool ascii_armor = true,
                       bool text_mode = true);
    bool importKeyFromFile(QString path);
    std::vector<GpgME::Key> getKeys(QString pattern_uid, bool remote = false, bool include_sigs = false,
                                    bool validate = false);
    QStringList getAllKeysId(bool remote = false, bool include_sigs = false, bool validate = false);

    bool setGpghome(QString path = QStandardPaths::writableLocation(
                                       QStandardPaths::AppDataLocation).append("/gpghome"));
};

#endif
