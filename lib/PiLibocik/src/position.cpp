#include <pilibocik/Position.h>

namespace PiLibocik{

    Position::Position(double lon, double lat):lon(lon),lat(lat){
    }

    double Position::getLon(){
        return lon;
    }

    double Position::getLat(){
        return lat;
    }

}
