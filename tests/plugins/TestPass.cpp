#include <QtTest/QtTest>

class TestPass: public QObject
{
Q_OBJECT
private slots:
    void toUpper();
};

void TestPass::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}

QTEST_MAIN(TestPass)
#include "TestPass.moc"