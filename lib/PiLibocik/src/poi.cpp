#include <pilibocik/poi.h>
#include <QStringList>

namespace PiLibocik{

Poi::Poi(double lon,double lat,QString name,int type, QList < QPair <QString, QString> > tags)
         :Position(lon,lat),name(name),type(type),tags(tags){
}

QString Poi::getName() const{
    return name;
}

QList <QPair <QString, QString> > Poi::getTags() const{
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

int Poi::isOpen(QDate now)
{
    int ret = 0;
    typedef  QPair <QString, QString> tagPair;
    QString openingHours;
    foreach(tagPair tag, tags)
    {
        if(tag.first=="opening_hours")
        {
           ret = 1;
           openingHours=tag.second;
           break;
        }
    }
    if(ret = 0)
        return 0;
    if(openingHours.compare("24/7") || openingHours.compare("7/24") || openingHours.compare("0-24"))
        return 1;

    QStringList openingHoursList = openingHours.split(';');
    int today = now.dayOfWeek();
    QMap<QString, int> dayMap;
    dayMap.insert("Mo",1);
    dayMap.insert("Tu",2);
    dayMap.insert("We",3);
    dayMap.insert("Th",4);
    dayMap.insert("Fr",5);
    dayMap.insert("Sa",6);
    dayMap.insert("Su",7);
//    foreach(QString hour, openingHoursList)
//    {
//        QList<int> days;
//        bool
//        foreach(QString dayStr, dayMap.keys())
//        {
//            if(hour.contains(dayStr)){
//                days.append(dayMap[dayStr]);
//            }
//        }
//    }

}



//#ifdef PiLibocik_WRITE_MODE
Poi::Poi(double lon,double lat,QString name,int type, QList < QPair <QString, QString> > tags, QString geohash)
    :Position(lon,lat),name(name),type(type),tags(tags),geohash(geohash){
}

QString Poi::getGeohash(){
    return geohash;
}
//#endif


}
