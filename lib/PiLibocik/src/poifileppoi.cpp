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
        Geohash geoH(geo.left(3));
        if(geoHashedPOIs.contains(geoH)){
            geoHashedPOIs.value(geoH)->push_back(&poi);
        }else{
            QVector <Poi*>* dupa=new QVector<Poi*>();
            dupa->push_back(&poi);
            geoHashedPOIs.insert(geoH,dupa);
        }
    }

    QMapIterator <Geohash ,QVector<Poi*> *> mIter(geoHashedPOIs);
    if(!mIter.hasNext()){
        //TODO CLEAR GOTO: ?
        qDebug()<<"Something wrong, NO POIs to SAVE";
        return;
    }


    QList<Geohash> geokeys=geoHashedPOIs.keys();
    qSort(geokeys);
    QListIterator<Geohash> listIterator(geokeys);


    QByteArray fileIndex,fileData;
    int size=3;
    //------------Index init-----------
    //Geohash size in bajts
    unsigned long long number=0;
    Geohash geo1(0),geo2(0);
    unsigned int index;
    unsigned int empty=0;
    while(listIterator.hasNext()){
        if(number==0){
            geo2=listIterator.next();
            QString n=geo2.toQString();
            //geohash size
            fileIndex.append((char)(n.length()));
            //First geohash
            fileIndex.append(n.toAscii());
            //TODO
            fileIndex.append(index);
        }
        geo1=geo2;
        geo2=listIterator.next();
        for(;geo1<geo2;geo1++){

            fileIndex.append(empty);
        }
        //TODO
        fileIndex.append(index);
    }


    //TODO SAVE DATA
    //QDataStream out(&file);
    //Magic number to know file TYPE
    //out << (quint32)0xABCDCCDD;

    //Clear
    //TODO Remove Vectors
}
#endif

}
