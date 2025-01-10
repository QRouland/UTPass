#ifndef TESTSUTILS_H
#define TESTSUTILS_H

#include <QObject>
#include <QUrl>
#include <QQuickWindow>

class TestsUtils : public QObject
{
    Q_OBJECT

public:
    TestsUtils() = default;
    ~TestsUtils() override = default;

    Q_INVOKABLE QString getTempPath();
};

#endif
