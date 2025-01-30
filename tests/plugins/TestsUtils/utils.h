#ifndef TESTSUTILS_H
#define TESTSUTILS_H

#include "passphraseprovider.h"
#include <QObject>
#include <QUrl>
#include <QQuickWindow>
#include <memory>

class TestsUtils : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<TesTPassphraseProvider> m_passphrase_povider;

public:
    TestsUtils();
    ~TestsUtils() override = default;

    Q_INVOKABLE QString getTempPath();
    Q_INVOKABLE bool fileExists(QUrl path);
    Q_INVOKABLE void copyFolder(QUrl sourceFolder, QUrl destFolder);
    Q_INVOKABLE QObject *getTestPassphraseProvider();

};

#endif
