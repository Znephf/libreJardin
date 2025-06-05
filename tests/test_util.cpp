#include <QtTest/QtTest>
#include <QLocale>
#include "../utilitaires/util.h"
#include "../graphic/background.h"
#include "../consts.h"
#include <QDir>

class TestUtil : public QObject
{
    Q_OBJECT
private slots:
    void afficheZeros_data();
    void afficheZeros();
    void afficheZerosDigits_data();
    void afficheZerosDigits();
    void apos_data();
    void apos();
    void getLocale();
    void backgroundValues();
    void constsValues();
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

void TestUtil::afficheZerosDigits_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("digits");
    QTest::addColumn<QString>("expected");

    QTest::newRow("three digits with decimal") << QString("1.2") << 3 << QString("1.200");
    QTest::newRow("three digits integer") << QString("3") << 3 << QString("3.000");
    QTest::newRow("one digit existing") << QString("2.34") << 1 << QString("2.34");
}

void TestUtil::afficheZerosDigits()
{
    QFETCH(QString, input);
    QFETCH(int, digits);
    QFETCH(QString, expected);
    QCOMPARE(util::afficheZeros(input, digits), expected);
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

void TestUtil::backgroundValues()
{
    background bg;
    bg.setType(1);
    QCOMPARE(bg.getType(), 1);

    QString filePath = QDir("..").filePath("jardin_type.png");
    bg.setPixFile(filePath);
    QCOMPARE(bg.getPixFile(), filePath);
    QVERIFY(bg.getBgWidth() > 0);
    QVERIFY(bg.getBgHeight() > 0);
}

void TestUtil::constsValues()
{
    QCOMPARE(Consts::APP_NAME, QString("LibreJardin"));
    QCOMPARE(Consts::SPACE_CASE, 14);
    QCOMPARE(Consts::SPACE_SNAP, 14);
    QCOMPARE(Consts::APP_DIR, QString("libreJardin/"));
    QCOMPARE(Consts::SHARE_DIR, QString("/usr/share/libreJardin/"));
    QCOMPARE(Consts::FILE_NAMEXML, QString("jardin.xml"));
    QCOMPARE(Consts::FILE_NAMESQL, QString("jardin.sqli"));
    QCOMPARE(Consts::DEFAULT_PLAN_IMAGE_FILE, QString("jardin_type.png"));
}

QTEST_MAIN(TestUtil)
#include "test_util.moc"
