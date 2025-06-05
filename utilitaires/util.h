#ifndef UTIL_H
#define UTIL_H
#include <QString>

class util
{
public:
    util();
    static QString apos(const QString texte);   // replace apostrophes for SQL queries
    static QString afficheZeros(QString texte); // zero-fill at the end of the string
    static QString getLocale();
};

#endif // UTIL_H
