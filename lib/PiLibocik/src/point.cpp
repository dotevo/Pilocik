#include <pilibocik/point.h>

namespace PiLibocik{

    Point::Point(double lon, double lat):lon(lon),lat(lat){
    }

    double Point::getLon(){
        return lon;
    }

    double Point::getLat(){
        return lat;
    }

    Geohash Point::getGeohash(int p){
        return Geohash::generateGeohash(lon,lat,p);
    }

}

