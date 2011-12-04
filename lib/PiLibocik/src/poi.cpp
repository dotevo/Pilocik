#include <pilibocik/poi.h>
#include <QStringList>
#include <QDebug>

namespace PiLibocik{

PoiDisplay::PoiDisplay(){
}

PoiDisplay::PoiDisplay(int type, bool display, int zoom, QString iconPath)
{
    this->type = type;
    this->display = display;
    this->zoom = zoom;
    this->iconPath = iconPath;
}

void PoiDisplay::setDisplay(bool display)
{
    this->display = display;
}

bool PoiDisplay::getDisplay()
{
    return display;
}

int PoiDisplay::getZoom()
{
    return zoom;
}

int PoiDisplay::getType()
{
    return type;
}

bool PoiDisplay::operator ==(const PoiDisplay &p) const
{
    return type == p.type;
}

QString PoiDisplay::getIconPath()
{
    return iconPath;
}

Poi::Poi(){
}

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

int Poi::isOpen(QDateTime now)
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
    if(ret == 0)
        return 0;
    if(openingHours=="24/7")
        return 1;

    QStringList openingHoursList = openingHours.split(';');
    int today = now.date().dayOfWeek();
    QMap<int,QString> dayMap;
    dayMap.insert(1,"Mo");
    dayMap.insert(2,"Tu");
    dayMap.insert(3,"We");
    dayMap.insert(4,"Th");
    dayMap.insert(5,"Fr");
    dayMap.insert(6,"Sa");
    dayMap.insert(7,"Su");
    foreach(QString hour, openingHoursList)
    {
        QList<int> days;
        int idxStart=-1;
        int idxEnd=-1;
        foreach(QString dayStr, dayMap.values())
        {
            if(hour.contains(dayStr)){
                days.append(dayMap.key(dayStr));
                idxStart=idxStart==-1?hour.indexOf(dayStr):idxStart;
                idxEnd=idxEnd==-1?idxStart+2:hour.indexOf(dayStr)+2;
            }
        }
        qSort(days);
        if(days.size()>2)
        {
            int cutIdx = hour.indexOf(dayMap[days.at(2)]);
            openingHoursList.append(hour.right(hour.size()-cutIdx));
            while(days.size()>2)
                days.removeLast();
        }
        if(hour.mid(idxStart, idxEnd-idxStart).contains("-"))
        {
            int dayMax = days.last();
            for(int i = days.first()+1; i<dayMax; i++)
                days.append(i);
        }
        if(!days.isEmpty() && !days.contains(today))
        {
            ret = 0;
            continue;
        }
        QString timeRange;
        if(idxStart==0)
            timeRange = hour.mid(idxEnd+1,hour.size()-idxEnd);
        else
            timeRange = hour.left(hour.size()-idxEnd+idxStart);
        QTime startTime, endTime, nowTime;
        nowTime = now.time();
        QChar separator;
        if(timeRange.contains(':'))
            separator = ':';
        if(timeRange.contains('.'))
            separator = '.';
        if(timeRange.split('-').size()<2)
            return -1;
        QStringList startTimeStr = timeRange.split('-').at(0).split(separator);
        QStringList endTimeStr = timeRange.split('-').at(1).split(separator);
        if(startTimeStr.size()<2 || endTimeStr.size()<2){
            ret = -1;
            continue;
        }
        if(endTimeStr.at(0)=="24"){
            endTimeStr.clear();
            endTimeStr.append("23");
            endTimeStr.append("59");
        }
        startTime = QTime(startTimeStr.at(0).toInt(), startTimeStr.at(1).toInt());
        endTime = QTime(endTimeStr.at(0).toInt(), endTimeStr.at(1).toInt());
        if(nowTime>=startTime && nowTime<=endTime){
            return 1;
        }
    }
    return ret == 0 ? 0 : -1;
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
