#ifndef GEOHASH_H
#define GEOHASH_H

#include <QString>
#include <QPair>

namespace PiLibocik{
    class Geohash
    {
    private:
        static const QString char_map;
    public:
        Geohash();
        QString next(QString Geohash);
        static QString generateGeohash(double lon, double lat, int precision);
    };
}

#endif // GEOHASH_H
