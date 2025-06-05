#ifndef UTIL_H
#define UTIL_H
#include <QString>

class util
{
public:
    util();
    static QString apos(const QString texte);   // replace apostrophes for SQL queries
    static QString afficheZeros(QString texte, int digits = 2); // zero-fill at the end of the string with configurable precision
    static QString getLocale();
};

#endif // UTIL_H
