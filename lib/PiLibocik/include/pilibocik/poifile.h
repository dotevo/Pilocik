#ifndef POIFILE_H
#define POIFILE_H

#include <QString>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>

namespace PiLibocik{

class PoiFile{
private:
    QString fileName;
public:
    void openFile(QString name);
    virtual QList<Poi> loadFromFile(BoundaryBox bbox)=0;

    #ifdef PiLibocik_WRITE_MODE
    virtual void saveToFile(QList<Poi>&pois)=0;
    #endif
};


}

#endif // POIFILE_H
