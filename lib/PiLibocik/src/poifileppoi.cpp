#include <pilibocik/poifileppoi.h>
#include <QVector>

namespace PiLibocik{
PoiFilePPOI::PoiFilePPOI()
{
}

void PoiFilePPOI::openFile(QString string){

}

QList<Poi> PoiFilePPOI::loadFromFile(BoundaryBox bbox){
    //int QDataStream::skipRawData ( int len ) skip bytes number
}

#ifdef PiLibocik_WRITE_MODE
void PoiFilePPOI::saveToFile(QList<Poi>&pois){
    file.open(QIODevice::WriteOnly);
    QMap <QString, QVector<Poi*>* > geoHashedPOIs;
    QListIterator <Poi> iter(pois);

    //Convert List to hashed map
    while(iter.hasNext()){
        Poi poi=iter.next();
        QString geo=poi.getGeohash();
        if(geoHashedPOIs.contains(geo)){
            geoHashedPOIs.take(geo)->push_back(&poi);
        }else{
            QVector <Poi*>* dupa=new QVector<Poi*>();
            dupa->push_back(&poi);
            geoHashedPOIs.insert(geo,dupa);
        }
    }

    //TODO SAVE DATA
    QDataStream out(&file);
    //Magic number to know file TYPE
    out << (quint32)0xABCDCCDD;

    //Clear
    //TODO Remove Vectors

    file.close();
}
#endif

}
