#ifndef POIFILEPPOI_H
#define POIFILEPPOI_H

#include <QFile>
#include <pilibocik/poifile.h>

namespace PiLibocik{

class PoiFilePPOI:public PoiFile
{
private:
    QFile file;
public:
    PoiFilePPOI();
    void openFile(QString string);
    QList<Poi> loadFromFile(BoundaryBox bbox);

    #ifdef PiLibocik_WRITE_MODE
    void saveToFile(QList<Poi>&pois);
    #endif

};

}

#endif // POIFILEPPOI_H
