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
void PoiFilePPOI::saveToFile(QString file,QList<Poi>&pois,QMap<int,QString> &types){
    QMap <Geohash, QVector<Poi*>* > geoHashedPOIs;
    QListIterator <Poi> iter(pois);
    //Convert List to hashed map
    while(iter.hasNext()){
        Poi *poi=(Poi*)&iter.next();
        QString geo=poi->getGeohash();
        Geohash geoH(geo.left(3));
        if(geoHashedPOIs.contains(geoH)){
            geoHashedPOIs.value(geoH)->push_back(poi);
        }else{
            QVector <Poi*>* dupa=new QVector<Poi*>();
            dupa->push_back(poi);
            geoHashedPOIs.insert(geoH,dupa);
        }
    }



    QMapIterator <Geohash ,QVector<Poi*> *> mIter(geoHashedPOIs);
    if(!mIter.hasNext()){
        goto clear;
    }
    {

        QList<Geohash> geokeys=geoHashedPOIs.keys();
        qSort(geokeys);



        QByteArray fileIndex,fileData;
        //------------Data Types-----------
        QList<int> typesList=types.keys();
        qSort(typesList);
        //Types count
        int typesC=typesList.size();
        fileData.append((char)typesList.size());    //fileDATA     1byte(n)
        QListIterator<int> typesListIter(typesList);        
        while(typesListIter.hasNext()){            
            QString s=types.value(typesListIter.next());
            fileData.append((char)s.length());      //fileDATA     1byte(m)*n
            fileData.append(s.toAscii());           //fileDATA     1byte*m
        }

        //------------Index init-----------
        QListIterator<Geohash> listIterator(geokeys);
        unsigned long long number=0;
        Geohash geo1(0),geo2(0);
        unsigned int empty=0;
        while(listIterator.hasNext()){            
            if(number==0){
                geo2=listIterator.next();
                QString n=geo2.toQString();
                //geohash size
                fileIndex.append((char)(n.length()));
                //First geohash
                fileIndex.append(n.toAscii());

                //Data (first block)
                QByteArray data=makeBlock(geoHashedPOIs.value(geo2),typesC);
                fileData.append(data);
                //index Counter
                fileIndex.append(fileData.size());
                number++;
            }
            geo1=geo2;
            geo2=listIterator.next();
            for(;geo1<geo2;geo1++){
                fileIndex.append(empty);
            }
            QByteArray data=makeBlock(geoHashedPOIs.value(geo2),typesC);
            fileData.append(data);
            //index Counter
            fileIndex.append(fileData.size());
        }
        qDebug()<<"File data size:"<<fileData.size()<<" Index file size:"<<fileIndex.size();

        QFile qfileData(file+".ppoi");
        QFile qfileIndex(file+".pidx");
        qfileData.open(QIODevice::WriteOnly);
        qfileIndex.open(QIODevice::WriteOnly);
        QDataStream outData(&qfileData);
         outData << fileData;
        QDataStream outIndex(&qfileIndex);
         outIndex << fileIndex;
        qfileData.close();
        qfileIndex.close();
    }
clear:
    QMapIterator <Geohash ,QVector<Poi*> *> delIter(geoHashedPOIs);
    while(delIter.hasNext()){
        delIter.next();
        delete delIter.value();
    }


}

QByteArray PoiFilePPOI::makeBlock(QVector<Poi*>*data,int types){
    if(data==0)
        return QByteArray();

    //---------Convert POI to table[type]=QVector<Poi*>*
    QVector<Poi*>*  table[types];
    for(int i=0;i<types;i++){
        table[i]=new QVector<Poi*>();
    }

    for(int i=0;i<data->size();i++){
        Poi *poi=data->at(i);
        qDebug()<<"CoNVER"<<poi->getLat();
        table[poi->getType()]->push_back(poi);
    }
    //---------Export data
    QByteArray index;

    QByteArray wtf;
    int byteCounter=types*sizeof(short);
    short empty=0;

    for(int i=0;i<types;i++){
        //POIs in type
        if(table[i]->size()>0){
            index.append((short)(byteCounter+wtf.size()));
            //POI in type
            wtf.append((char)(table[i]->size()));
            for(int j=0;j<table[i]->size();j++){
                Poi *poi=table[i]->at(j);                
                //Add LOC
                wtf.append(poi->getLon());
                wtf.append(poi->getLat());                                

                //Add Name
                QString name=poi->getName();
                wtf.append((char)name.length());
                wtf.append(name.toAscii());

                //Add Tags count
                QList <QPair<QString,QString > > list=poi->getTags();
                wtf.append((char)list.count());

                //Add Tags
                QListIterator <QPair<QString,QString > > tagsIter(list);
                while(tagsIter.hasNext()){
                    QPair<QString,QString> pair=tagsIter.next();
                    QString valueP=pair.first+"="+pair.second;
                    wtf.append((char)valueP.length());
                    QByteArray aZ=valueP.toAscii();
                    wtf.append(aZ);
                }
            }
        }else{
            //empty index
            index.append(empty);
        }
    }

    //Clear
    for(int i=0;i<types;i++){
        delete table[i];
    }

    index.append(wtf);
    return index;
}

#endif

}
