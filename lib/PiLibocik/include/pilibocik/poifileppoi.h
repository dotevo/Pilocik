#ifndef POIFILEPPOI_H
#define POIFILEPPOI_H

#include <QFile>
#include <pilibocik/poifile.h>

namespace PiLibocik{

class PoiFilePPOI:public PoiFile
{
public:
    PoiFilePPOI();
    QList<Poi> loadFromFile(QString file,BoundaryBox &bbox);

    #ifdef PiLibocik_WRITE_MODE
    void saveToFile(QString file,QList<Poi>&pois,QMap<int,QString> &types);
    void makeBlock(QDataStream &stream,QVector<Poi*>*data,int types);
    #endif

};

}

#endif // POIFILEPPOI_H
