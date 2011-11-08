#ifndef POIFILE_H
#define POIFILE_H

#include <QString>
#include <QFile>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>

namespace PiLibocik{

class PoiFile{
public:
    virtual void openFile(QString string)=0;
    virtual QList<Poi> loadFromFile(BoundaryBox bbox)=0;

    #ifdef PiLibocik_WRITE_MODE
    virtual void saveToFile(QList<Poi>&pois)=0;
    #endif
};


}

#endif // POIFILE_H
