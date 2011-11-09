#ifndef POIFILE_H
#define POIFILE_H

#include <QString>
#include <QFile>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>

namespace PiLibocik{

class PoiFile{
public:
    virtual QList<Poi> loadFromFile(QFile &file,BoundaryBox &bbox)=0;

    #ifdef PiLibocik_WRITE_MODE
    virtual void saveToFile(QFile &file,QList<Poi>&pois,QMap<int,QString> &Types)=0;
    #endif
};


}

#endif // POIFILE_H
