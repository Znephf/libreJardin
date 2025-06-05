#include <QtTest/QtTest>
#include <QLocale>
#include "../utilitaires/util.h"

class TestUtil : public QObject
{
    Q_OBJECT
private slots:
    void afficheZeros_data();
    void afficheZeros();
    void apos_data();
    void apos();
    void getLocale();
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

void TestUtil::apos_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("no quote") << QString("Hello") << QString("Hello");
    QTest::newRow("single quote") << QString("O'Brien") << QString("O''Brien");
    QTest::newRow("multiple quotes") << QString("'quoted'") << QString("''quoted''");
}

void TestUtil::apos()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);
    QCOMPARE(util::apos(input), expected);
}

void TestUtil::getLocale()
{
    QCOMPARE(util::getLocale(), QLocale::system().name());
}

QTEST_MAIN(TestUtil)
#include "test_util.moc"
