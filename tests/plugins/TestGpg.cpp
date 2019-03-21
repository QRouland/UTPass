#include <QtTest/QtTest>

class TestGpg: public QObject
{
Q_OBJECT
private slots:
    void toUpper();
};

void TestGpg::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(TestGpg)
#include "TestGpg.moc"