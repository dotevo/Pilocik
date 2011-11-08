#include <pilibocik/poi.h>

namespace PiLibocik{

Poi::Poi(double lon,double lat,QString name,int type, QList < QPair <QString, QString> > tags)
         :Point(lon,lat),name(name),type(type),tags(tags){
}

QString Poi::getName() const{
    return name;
}

QList < QPair <QString, QString> > Poi::getTags() const{
    return tags;
}

int Poi::getType() const{
    return type;
}

void Poi::setTypes(QMap <int,QString> typs){
    this->types=typs;

}

QString Poi::getTypeName(int type){
    return this->types.value(type,QString());
}




#ifdef PiLibocik_WRITE_MODE
Poi::Poi(double lon,double lat,QString name,int type, QList < QPair <QString, QString> > tags, QString geohash)
    :Point(lon,lat),name(name),type(type),tags(tags),geohash(geohash){
}

QString Poi::getGeohash(){
    return geohash;
}
#endif

}
