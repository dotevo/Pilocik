#ifndef POIFILEPPOI_H
#define POIFILEPPOI_H

#include <QFile>
#include <pilibocik/poifile.h>

namespace PiLibocik{

class PoiFilePPOI:public PoiFile
{
public:
    PoiFilePPOI();
    QList<Poi> loadFromFile(QFile &file,BoundaryBox &bbox);

    #ifdef PiLibocik_WRITE_MODE
    void saveToFile(QFile&file,QList<Poi>&pois,QMap<int,QString> &types);
    QByteArray makeBlock(QVector<Poi*>*data,int types);
    #endif

};

}

#endif // POIFILEPPOI_H
