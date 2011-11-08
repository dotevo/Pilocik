#include <pilibocik/geohash.h>

uint qHash(PiLibocik::Geohash &key){
    return qHash( key.toString() );
}

namespace PiLibocik{
    const QString Geohash::char_map =  "0123456789bcdefghjkmnpqrstuvwxyz";

    Geohash::Geohash(int size):geohashSize(size){
        geohashValue=new char[size];
    }

    QString Geohash::toString(){
        QString ret;
        for(int i=0;i<geohashSize;i++){
            ret+=char_map[geohashValue[i]];
        }
        return ret;
    }

    void Geohash::operator++(int){
        for(int i=geohashSize-1;i>=0;i--){
            geohashValue[i]++;
            if(geohashValue[i]>31&&i>0){
                geohashValue[i]=0;
            }else if(geohashValue[i]>31&&i==0){
                geohashValue[i]=0;
            }else{
                break;
            }
        }
    }

    void Geohash::operator--(int){
        for(int i=geohashSize-1;i>=0;i--){
            geohashValue[i]--;
            if(geohashValue[i]==-1&&i>0){
                geohashValue[i]=31;
            }else if(geohashValue[i]==-1&&i==0){
                geohashValue[i]=31;
            }else{
                break;
            }
        }
    }

    bool Geohash::operator>(Geohash &q){
        for(int i=0;i<geohashSize&&i<q.geohashSize;i++){
            if(geohashValue[i]>q.geohashValue[i]){
                return true;
            }
        }
        return false;
    }

    bool Geohash::operator<(Geohash &q){
        for(int i=0;i<geohashSize&&i<q.geohashSize;i++){
            if(geohashValue[i]<q.geohashValue[i]){
                return true;
            }
        }
        return false;
    }

    bool Geohash::operator==(Geohash &q){
        if(geohashSize!=q.geohashSize)
            return false;
        for(int i=0;i<geohashSize&&i<q.geohashSize;i++){
            if(geohashValue[i]!=q.geohashValue[i]){
                return false;
            }
        }
        return true;
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
