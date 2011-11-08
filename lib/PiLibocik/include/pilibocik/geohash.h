#ifndef GEOHASH_H
#define GEOHASH_H

#include <QString>
#include <QPair>

namespace PiLibocik{
    class Geohash
    {
    public:
        Geohash();
        QString generateGeohash(double lon, double lat, int precision);
    };
}

#endif // GEOHASH_H
