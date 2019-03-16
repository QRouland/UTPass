#include <QDebug>
#include <QFile>
#include "pass.h"
#include "gpgme++/global.h"
#include "qgpgme/encryptjob.h"
#include "qgpgme/decryptjob.h"
#include "qgpgme/keylistjob.h"


Pass::Pass() {
    qDebug() << "Initializing GpgMe";
    /*auto err = GpgME::initializeLibrary(GpgME::OpenPGP);
    if(err) {
        qFatal("Fatal error when initializing GpgMe");
    }*/
}

void Pass::speak() {
    qDebug() << "Starting app from main.cpp";
}

QString Pass::decrypt(const QByteArray cipherText) {
    /*auto decJob = QGpgME::openpgp()->decryptJob();
    auto ctx = QGpgME::Job::context(decJob);

    TestPassphraseProvider provider;
    ctx->setPassphraseProvider(&provider);
    ctx->setPinentryMode(Context::PinentryLoopback);

    QByteArray plainText;
    decJob->exec(cipherText, plainText);

    return QString::fromUtf8(plainText);*/
}

QString Pass::decrypt_file(const QString path) {
    /*QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qErrnoWarning("Can't open the File");
        return nullptr;
    }
    QByteArray plainText = file.readAll();
    return Pass::decrypt(plainText);*/
}

QByteArray Pass::encrypt(const QString str) {
    /*auto listjob = QGpgME::openpgp()->keyListJob(false, false, false);
    std::vector<GpgME::Key> keys;
    auto keylistresult = listjob->exec(QStringList() << QStringLiteral("alfa@example.net"),
                                       false, keys);
    auto job = QGpgME::openpgp()->encryptJob(/*ASCII Armor *//*true, /* Textmode *//* true);
    QByteArray cipherText;
    auto result = job->exec(keys, QStringLiteral("Hello World").toUtf8(), GpgME::Context::AlwaysTrust, cipherText);
    return cipherText;*/
}

bool Pass::encrypt_file(const QString str, const QString path) {
    /*QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qErrnoWarning("Can't open the File");
        return false;
    }
    file.write(Pass::encrypt(str));
    return true;*/
}
