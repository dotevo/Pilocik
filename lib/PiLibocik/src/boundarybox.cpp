#include <pilibocik/boundarybox.h>
#include <QDebug>

namespace PiLibocik{
BoundaryBox::BoundaryBox(Point p1,Point p2):p1(p1),p2(p2){
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
    //TODO bbox to geohashes
    QMap<QString, Geohash> ret;
    for(double lon=p1.getLon(); lon < p2.getLon(); lon+=spatialError[precision].first)
    {
        for(double lat=p1.getLat(); lat < p2.getLat(); lat+=spatialError[precision].second)
        {
            Geohash geo(lon, lat, precision);
            if(!ret.contains(geo.toQString()))
            {
                ret.insert(geo.toQString(), geo);
                qDebug()<<geo.toQString();
            }

        }
    }
    return ret.values();
}

}
