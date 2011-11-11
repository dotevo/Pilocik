#ifndef GEOHASH_H
#define GEOHASH_H

#include <QString>
#include <QPair>
#include <QByteArray>
#include <QHash>

namespace PiLibocik{
    class Geohash
    {
    private:
        static const QString char_map;
        int geohashSize;
        char *geohashValue;

        void init(QString geohash);
    public:
        Geohash(int precision);
        Geohash(QString geohash);
        Geohash(double lon,double lat,int precision);
        ~Geohash();


        QString toQString();
        static QString generateGeohash(double lon, double lat, int precision);

        void operator++(int);
        void operator--(int);
        unsigned long long  operator-(Geohash &q);
        bool operator>(Geohash &q);
        bool operator<(Geohash &q);
        bool operator==(Geohash &q);
    };
}

inline bool operator<(const  PiLibocik::Geohash &e1, const PiLibocik::Geohash &e2){
    return ((PiLibocik::Geohash)e1).operator<((PiLibocik::Geohash&)e2);
}

inline bool caseInsensitiveLessThan(const PiLibocik::Geohash &s1, const PiLibocik::Geohash &s2)
{
    return s1<s2;
}

uint qHash(PiLibocik::Geohash &key);

#endif // GEOHASH_H
