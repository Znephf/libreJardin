#include <QtTest/QtTest>
#include "../utilitaires/util.h"

class TestUtil : public QObject
{
    Q_OBJECT
private slots:
    void afficheZeros_data();
    void afficheZeros();
};

void TestUtil::afficheZeros_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("with decimal") << QString("1.2") << QString("1.20");
    QTest::newRow("integer") << QString("3") << QString("3.00");
}

void TestUtil::afficheZeros()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);
    QCOMPARE(util::afficheZeros(input), expected);
}

QTEST_MAIN(TestUtil)
#include "test_util.moc"
