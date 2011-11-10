#ifndef POIFILEPPOI_H
#define POIFILEPPOI_H

#include <QFile>
#include <pilibocik/poifile.h>

namespace PiLibocik{

class PoiFilePPOI:public PoiFile
{
public:
    PoiFilePPOI();
    QMap< int,QString > loadPOIsTypesFromFile(QString file);
    QList<Poi> loadPOIsFromFile(QString file,BoundaryBox &bbox,int PoiType);


    #ifdef PiLibocik_WRITE_MODE
    void saveToFile(QString file,QList<Poi>&pois,QMap<int,QString> &types);
    void makeBlock(QDataStream &stream,QVector<Poi*>*data,int types);
    #endif
private:
    QList<Poi> loadBlock(QDataStream &inData,int position,int poiType,int typeSize);
    QList<Poi> loadPoisInType(QDataStream &inData,int position,int type);
};

}

#endif // POIFILEPPOI_H
