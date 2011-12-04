#include <pilibocik/boundarybox.h>
#include <QDebug>

namespace PiLibocik{
BoundaryBox::BoundaryBox(Position p1,Position p2):p1(p1),p2(p2){
    precision = -1;
    spatialError.insert(1, QPair<double,double>(23,23));
    spatialError.insert(2, QPair<double,double>(5.6,2.8));
    spatialError.insert(3, QPair<double,double>(0.7,0.7));
    spatialError.insert(4, QPair<double,double>(0.18,0.087));
    spatialError.insert(5, QPair<double,double>(0.022,0.022));
    spatialError.insert(6, QPair<double,double>(0.0055,0.0027));
    spatialError.insert(7, QPair<double,double>(0.00068,0.00068));
    spatialError.insert(8, QPair<double,double>(0.00017,0.000085));
}

QList <Geohash> BoundaryBox::getGeohashesIn(int precision){
    this->precision = precision;
    QMap<QString, Geohash> ret;
    double lon=p1.getLon();
    while(1)
    {
        double lat=p1.getLat();
        while(1)
        {
            Geohash geo(lon, lat, precision);
            if(!ret.contains(geo.toQString()))
            {
                ret.insert(geo.toQString(), geo);
            }
            if(lat > p2.getLat()) break;
            lat+=spatialError[precision].second;
        }
        if(lon > p2.getLon()) break;
        lon+=spatialError[precision].first;
    }
    return ret.values();
}

QPair<double,double> BoundaryBox::getCurrentError()
{
    return spatialError[precision];
}

}
