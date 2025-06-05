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

QString util::afficheZeros(QString texte)
{
    texte.replace(QString(","), QString("."));
    int index = texte.indexOf('.');
    if (index >= 0)
    {
        int digits = texte.length() - index - 1;

        if (digits == 0)
        {
            texte += "00";
        }
        else if (digits == 1)
        {
            texte += '0';
        }
        // if digits >= 2 do nothing
    }
    else
    {
        texte += ".00";
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
