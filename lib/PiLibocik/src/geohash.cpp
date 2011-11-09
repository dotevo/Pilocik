#include <pilibocik/geohash.h>
#include <QDebug>

uint qHash(PiLibocik::Geohash &key){
    return qHash( key.toQString() );
}

namespace PiLibocik{
    const QString Geohash::char_map =  "0123456789bcdefghjkmnpqrstuvwxyz";

    Geohash::Geohash(int size):geohashSize(size){
        geohashValue=new char[size];
    }

    Geohash::Geohash(QString gh){
        init(gh);
    }

    Geohash::Geohash(double lon,double lat,int s){
        QString sl=generateGeohash(lon,lat,s);
        init(sl);
    }


    void Geohash::init(QString gh){
        geohashSize=gh.length();
        geohashValue=new char[geohashSize];
        for(int i=0;i<geohashSize;i++)
            for(int j=0;j<32;j++){
                if(gh.at(i)==char_map[j]){
                    geohashValue[i]=j;
                    j=32;
                }
            }

    }

    QString Geohash::toQString(){
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
            if((short)geohashValue[i]>(short)q.geohashValue[i]){
                return true;
            }else if((short)geohashValue[i]<(short)q.geohashValue[i])
                return false;
        }
        return false;
    }

    bool Geohash::operator<(Geohash &q){
        for(int i=0;i<geohashSize&&i<q.geohashSize;i++){
            if((short)geohashValue[i]<(short)q.geohashValue[i]){
                return true;
            }else if((short)geohashValue[i]>(short)q.geohashValue[i])
                return false;
        }
        return false;
    }

    unsigned long long Geohash::operator-(Geohash &q){
        if(q.geohashSize!=geohashSize)return -1;
        long long ret=0;
        for(int i=geohashSize-1;i>=0;i--){
            ret+=(geohashValue[i]-q.geohashValue[i])<<(5*(geohashSize-1-i));
        }
        return ret;
    }


    bool Geohash::operator==(Geohash &q){
        if(geohashSize!=q.geohashSize){
            return false;
            qDebug()<<"WTF";
        }
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
