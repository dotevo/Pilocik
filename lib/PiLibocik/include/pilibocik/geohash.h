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
    public:
        Geohash(int precision);

        QString toString();
        static QString generateGeohash(double lon, double lat, int precision);

        void operator++(int);
        void operator--(int);
        bool operator>(Geohash &q);
        bool operator<(Geohash &q);
        bool operator==(Geohash &q);
    };
}

uint qHash(PiLibocik::Geohash &key);

#endif // GEOHASH_H
