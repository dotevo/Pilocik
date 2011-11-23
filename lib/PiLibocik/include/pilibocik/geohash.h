#ifndef PILIBOCIK_GEOHASH_H
#define PILIBOCIK_GEOHASH_H

#include <QString>
#include <QPair>
#include <QByteArray>
#include <QHash>

namespace PiLibocik{
    class Geohash
    {
    private:
        static const QString char_map;


        void init(QString geohash);
    public:
        int geohashSize;
        char *geohashValue;

        Geohash();
        Geohash(int precision);
        Geohash(QString geohash);
        Geohash(const Geohash &geohash);
        Geohash(double lon,double lat,int precision);
        Geohash(char *n,int size);
        ~Geohash();


        QString toQString();
        static QString generateGeohash(double lon, double lat, int precision);

        void operator++(int);
        void operator--(int);
        unsigned long long  operator-(Geohash &q);
        bool operator>(Geohash &q);        
        bool operator<(Geohash &q);
        bool operator<(const Geohash &q);
        bool operator==(Geohash &q);
        Geohash &operator=(Geohash &q);
        Geohash &operator=(const Geohash &q);
    };
}

inline bool operator<(const  PiLibocik::Geohash &e1, const PiLibocik::Geohash &e2){
    for(int i=0;i<e1.geohashSize&&i<e2.geohashSize;i++){
        if((short)e1.geohashValue[i]<(short)e2.geohashValue[i]){
            return true;
        }else if((short)e1.geohashValue[i]>(short)e2.geohashValue[i])
            return false;
    }
    return false;
}

inline bool caseInsensitiveLessThan(const PiLibocik::Geohash &s1, const PiLibocik::Geohash &s2)
{
    return s1<s2;
}


uint qHash(PiLibocik::Geohash &key);

#endif // GEOHASH_H
