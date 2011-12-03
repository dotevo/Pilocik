#include <pilibocik/position.h>

#include <QtCore/qmath.h>

namespace PiLibocik{

static double earthRadius = 3958.75;

    Position::Position():lon(0),lat(0){
    }

    Position::Position(double lon, double lat):lon(lon),lat(lat){
    }

    double Position::getLon(){
        return lon;
    }

    void Position::setLon(double lon){
        this->lon = lon;
    }

    double Position::getLat(){
        return lat;
    }

    void Position::setLat(double lat){
        this->lat = lat;
    }

    Geohash Position::getGeohash(int p){
        return Geohash::generateGeohash(lon,lat,p);
    }

    double Position::getDistance(Position p){
       qreal dLat = (lat-p.lat) * 0.0174532925;
       qreal dLon = (lon-p.lon) * 0.0174532925;
       qreal a = qSin(dLat/2) * qSin(dLat/2) +
                  qCos(p.lat * 0.0174532925) * qCos(lat * 0.0174532925) *
                  qSin(dLon/2) * qSin(dLon/2);
       qreal c = 2 * qAtan2(qSqrt(a), qSqrt(1-a));
       qreal dist = earthRadius * c;

       return (double) dist;
    }

    QList< Position > Position::getPositionsAround(QPair< double, double > pair){
        QList< Position > posList;

        //double tmpLon = lon-(2*pair.first);
        //double tmpLat = lat-(2*pair.second);
        double tmpLon = lon;
        double tmpLat = lat;
        for(int i=0; i<1; ++i) {
            for(int j=0; j<1; ++j) {
                posList.append(Position(tmpLon, tmpLat));
                tmpLon += pair.first;
            }
            tmpLat += pair.second;
        }

        return posList;
    }

}
