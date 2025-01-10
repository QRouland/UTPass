#ifndef GPG_H
#define GPG_H

#include <memory>
#include <QQuickWindow>
#include <gpgme++/context.h>
#include <qgpgme/changeownertrustjob.h>


using namespace GpgME;

class Gpg
{
private:
    Gpg();

    QObject *m_window;

    QString findCommandPath(const QString &command);
    QString initGpgHome();
    QString initGpgExec();
    void initGpgConfig();

public:
    ~Gpg() = default;

    static std::shared_ptr<Gpg> instance()
    {
        static std::shared_ptr<Gpg> s{new Gpg};
        return s;
    }
    Gpg(Gpg const &) = delete;
    void operator=(Gpg const &) = delete;

    void setWindow(QObject *window)
    {
        m_window = window;
    };

    QObject *getWindow()
    {
        return m_window;
    };


    QPair<Error, std::vector<Key >> getAllKeys(bool remote = false, bool include_sigs = {}, bool
                                                  validate = false);
    QPair<Error, std::vector<Key>> getKeys( QString pattern_uid,  bool remote = false,
                                            bool include_sigs = false,
                                            bool validate = false);
    QPair<Error, Key> getKey( QString uid,  bool remote = false,  bool include_sigs = false,
                              bool validate = false);
    QPair<Error, QString>  decrypt( QByteArray cipherText);
    QPair<Error, QString> decryptFromFile( QString path);
    QPair<Error, QByteArray> encrypt( QString str,  QString uid,  bool ascii_armor = true,
                                      bool text_mode = true);
    Error encryptToFile( QString str,  QString path,  QString uid,  bool ascii_armor = true,
                         bool text_mode = true);
    Error importKeysFromFile( QString path);
    Error deleteKeyId( QString uid);
};

#endif
