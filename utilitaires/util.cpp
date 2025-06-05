#include "util.h"
#include <QCoreApplication>
#include <QDebug>
#include <qtranslator.h>

util::util()
{
}

QString util::apos(QString texte)
{
    texte.replace(QString("'"), QString("''")); //replacement of apostrophes
    QString texteModif = texte.toUtf8();        //convert
    return texteModif;
}

QString util::afficheZeros(QString texte, int digits)
{
    // replace comma with dot for decimal separator
    texte.replace(QString(","), QString("."));
    int index = texte.indexOf('.');

    if (index >= 0)
    {
        int currentDigits = texte.length() - index - 1;
        while (currentDigits < digits)
        {
            texte += '0';
            ++currentDigits;
        }
        // keep existing digits if there are more than requested
    }
    else
    {
        // no decimal part - add one filled with zeros
        texte += '.' + QString(digits, '0');
    }
    return texte;
}

QString util::getLocale()
{
    // retrieve system locale
    QString locale = QLocale::system().name();

    // QString locale = "en_EN";
    // qDebug() << " locale util " << locale;
    return locale;
}
