#include <pilibocik/geohash.h>

namespace PiLibocik{
    const QString Geohash::char_map =  "0123456789bcdefghjkmnpqrstuvwxyz";

    Geohash::Geohash()
    {
    }

    QString Geohash::next(QString Geohash){
        return QString("TODO");
    }

    QString Geohash::generateGeohash(double lng, double lat, int precision)
    {

        QChar hash[precision+1];

        if(lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {

            int limiter = precision * 5.0;

            QPair<double,double> lat_interval = QPair<double,double>(-90, 90);
            QPair<double,double> lng_interval = QPair<double,double>(-180, 180);

            QPair<double,double> *interval;
            double coord, mid;
            int is_even = 1;
            unsigned int hashChar = 0;
            int i;
            for(i = 1; i <= limiter; i++) {

                if(is_even) {
                    interval = &lng_interval;
                    coord = lng;
                }
                else {
                    interval = &lat_interval;
                    coord = lat;
                }

                mid = (interval->first + interval->second) / 2.0;
                hashChar = hashChar << 1;

                if(coord > mid) {
                    interval->first = mid;
                    hashChar |= 0x01;
                }
                else
                    interval->second = mid;

                if(!(i % 5)) {
                    hash[(i - 1) / 5] = char_map.at(hashChar);
                    hashChar = 0;
                }
                is_even = !is_even;
            }
        }
        return QString(hash,precision);
    }
}
