#include <pilibocik/boundarybox.h>

namespace PiLibocik{
BoundaryBox::BoundaryBox(Point p1,Point p2):p1(p1),p2(p2){
}



QList <Geohash> BoundaryBox::getGeohashesIn(int precision){
    //TODO bbox to geohashes
    Geohash geo1(p1.getLon(),p1.getLat(),precision);
    Geohash geo2(p2.getLon(),p2.getLat(),precision);
    QList <Geohash> ret;
    ret.append(geo1);
    if(!(geo1==geo2))
        ret.append(geo2);
    return ret;
}

}
