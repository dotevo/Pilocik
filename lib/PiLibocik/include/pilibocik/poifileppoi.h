#ifndef PILIBOCIK_POIFILEPPOI_H
#define PILIBOCIK_POIFILEPPOI_H

#include <QFile>
#include <pilibocik/poifile.h>

namespace PiLibocik{

/**
 * @brief
 *
 * @class PoiFilePPOI poifileppoi.h "include/pilibocik/poifileppoi.h"
 */
class PoiFilePPOI:public PoiFile
{
public:
/**
 * @brief
 *
 * @fn PoiFilePPOI
 */
    PoiFilePPOI();
    /**
     * @brief
     *
     * @fn loadPOIsTypesFromFile
     * @param file
     */
    QMap< int,QString > loadPOIsTypesFromFile(QString file);
    /**
     * @brief
     *
     * @fn loadPOIsFromFile
     * @param file
     * @param bbox
     * @param PoiType
     */
    QList<Poi> loadPOIsFromFile(QString file,BoundaryBox &bbox,int PoiType);


    #ifdef PiLibocik_WRITE_MODE
    void saveToFile(QString file,QList<Poi>&pois,QMap<int,QString> &types);
    void makeBlock(QDataStream &stream,QVector<Poi*>*data,int types);
    #endif
private:
    /**
     * @brief
     *
     * @fn loadBlock
     * @param inData
     * @param position
     * @param poiType
     * @param typeSize
     */
    QList<Poi> loadBlock(QDataStream &inData,int position,int poiType,int typeSize);
    /**
     * @brief
     *
     * @fn loadPoisInType
     * @param inData
     * @param position
     * @param type
     */
    QList<Poi> loadPoisInType(QDataStream &inData,int position,int type);
};

}

#endif // POIFILEPPOI_H
