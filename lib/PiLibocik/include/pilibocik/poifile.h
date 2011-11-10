#ifndef POIFILE_H
#define POIFILE_H

#include <QString>
#include <QFile>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>

namespace PiLibocik{

class PoiFile{
public:
    virtual QMap< int,QString > loadPOIsTypesFromFile(QString file)=0;
    virtual QList<Poi> loadPOIsFromFile(QString file,BoundaryBox &bbox,int PoiType)=0;

    #ifdef PiLibocik_WRITE_MODE
    virtual void saveToFile(QString file,QList<Poi>&pois,QMap<int,QString> &Types)=0;
    #endif
};


}

#endif // POIFILE_H
