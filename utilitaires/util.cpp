#include "util.h"

util::util()
{
}

QString util::apos(QString texte)
{
    texte.replace(QString("'"), QString("''")); //remplacement des apostrophes
    QString texteModif = texte.toUtf8();        //conversion
    return texteModif;
}

QString util::afficheZeros(QString texte)
{
    texte.replace(QString(","), QString("."));
    int index    = texte.indexOf(".");
    int longueur = texte.length();
    int position = longueur - index;

    if (index > 0)
    {
        if (position == 1)
        {
            texte = texte + "00";
        }
        if (position >= 2)
        {
            texte = texte + "0";
        }
    }
    else
    {
        texte = texte + ".00";
    }
    return texte;
}
