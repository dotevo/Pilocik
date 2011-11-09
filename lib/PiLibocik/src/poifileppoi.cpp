#include <pilibocik/poifileppoi.h>
#include <pilibocik/geohash.h>
#include <QVector>
#include <QMapIterator>
#include <QDebug>


namespace PiLibocik{
PoiFilePPOI::PoiFilePPOI()
{
}


QList<Poi> PoiFilePPOI::loadFromFile(QFile &file,BoundaryBox& bbox){
    //int QDataStream::skipRawData ( int len ) skip bytes number
}

#ifdef PiLibocik_WRITE_MODE
void PoiFilePPOI::saveToFile(QFile &file,QList<Poi>&pois){
    file.open(QIODevice::WriteOnly);
    QMap <Geohash, QVector<Poi*>* > geoHashedPOIs;
    QListIterator <Poi> iter(pois);

    //Convert List to hashed map
    while(iter.hasNext()){
        Poi poi=iter.next();
        QString geo=poi.getGeohash();
        Geohash geoH(geo.left(5));
        if(geoHashedPOIs.contains(geoH)){
            geoHashedPOIs.take(geoH)->push_back(&poi);
        }else{
            QVector <Poi*>* dupa=new QVector<Poi*>();
            dupa->push_back(&poi);
            geoHashedPOIs.insert(geoH,dupa);
        }
    }

    QMapIterator <Geohash ,QVector<Poi*> *> mIter(geoHashedPOIs);
    if(mIter.hasNext()){
        mIter.next();
        Geohash min=mIter.key();
        Geohash max=mIter.key();
        while(mIter.hasNext()){
            mIter.next();
            Geohash geoHl=mIter.key();
            if(min>geoHl)
                min=geoHl;
            if(max<geoHl)
                max=geoHl;
        }
        qDebug()<<"Min geohash:"<<min.toString()<<" Max geohash:"<<max.toString();
        qDebug()<<(max-min);

    }else{
        qDebug()<<"Something wrong, NO POIs to SAVE";
    }


    //TODO: FOR FROM MIN to MAX get from QMAP to Vector
    //TODO: SAVE Vector

    //TODO SAVE DATA
    //QDataStream out(&file);
    //Magic number to know file TYPE
    //out << (quint32)0xABCDCCDD;

    //Clear
    //TODO Remove Vectors

    //file.close();
}
#endif

}
