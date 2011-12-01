#include <pilibocik/position.h>

namespace PiLibocik{

    Position::Position():lon(0),lat(0){
    }

    Position::Position(double lon, double lat):lon(lon),lat(lat){
    }

    double Position::getLon(){
        return lon;
    }

    double Position::setLon(double lon){
        this->lon = lon;
    }

    double Position::getLat(){
        return lat;
    }

    double Position::setLat(double lat){
        this->lat = lat;
    }

    Geohash Position::getGeohash(int p){
        return Geohash::generateGeohash(lon,lat,p);
    }

    double Position::getSimpleDistance(Position p){
        double la=p.getLat()-getLat();
        double lo=p.getLon()-getLon();
        return la*la+lo*lo;
    }

}
