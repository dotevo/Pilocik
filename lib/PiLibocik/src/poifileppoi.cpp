#include <pilibocik/poifileppoi.h>
#include <pilibocik/geohash.h>
#include <QVector>
#include <QMapIterator>
#include <QDebug>
#include <QStringList>
#include <QBuffer>

/*Index file format
  geohash size 1byte(n)     -quint8
  geohash size 8byte(p)     -unsigned long long
  firstgeohash 1byte*(n)    -Asci
  index        4byte*p      -int
  */



namespace PiLibocik{
PoiFilePPOI::PoiFilePPOI()
{
}


QList<Poi> PoiFilePPOI::loadFromFile(QString file,BoundaryBox& bbox){
    QList <Poi> ret;
    //Load index File
    QFile qfileIndex(file+".pidx");
    qfileIndex.open(QIODevice::ReadOnly);
    QDataStream in(&qfileIndex);

    //LOAD INDEX DATA----------------
    quint8 geoHashSize=0;
    quint64 geoHashsCount=0;
    in >> geoHashSize;
    char *firstGeo;
    in >> geoHashsCount;
    in.readBytes(firstGeo,(uint&)geoHashSize);
    QString s(firstGeo);
    Geohash geoFirst(s);
    delete firstGeo;

    //TODO bbox to geohash!
    //in.skipRawData((downloadRegion-geoFirst)*4);
    Geohash downloadRegion=geoFirst;
    qint64 index;
    in >> index;
    //If empty
    qDebug()<<index;
    if(index==0)
        return ret;

    //LOAD DATA----------------------
    QFile qfileData(file+".ppoi");
    qfileData.open(QIODevice::ReadOnly);
    QDataStream inData(&qfileData);
    //load data types
    quint8 typesC;
    inData>>typesC;
    QMap <int,QString> types;
    for(int i=0;i<typesC;i++){
        quint8 size=0;
        inData>>size;
        char n[size];
        inData.readRawData((char*)&n,(int)size);
        QString name(n);
        name.resize(size);
        qDebug()<<name.length();
        types.insert(i,name);
    }
    //load bloack
    inData.skipRawData(index-inData.device()->pos());
    //TODO TYPES INDEX
        //SKIP TYPES
    inData.skipRawData(typesC*sizeof(quint16));

    //POIs Count
    quint16 pois_C;
    inData>>pois_C;
    //Get POIs
    for(int i=0;i<pois_C;i++){
        double lon,lat;
        quint8 nameS;
        inData>>lon>>lat>>nameS;
        char n[nameS];
        inData.readRawData((char*)&n,(int)nameS);
        QString name(n);
        name.resize(nameS);

        //Tags
        quint8 tagsC;
        inData>>tagsC;
        QList <QPair <QString,QString > > tags;
        for(int j=0;j<tagsC;j++){
            quint8 tagSize;
            inData>>tagSize;
            char tagChar[tagSize];
            inData.readRawData((char*)&tagChar,(int)tagSize);
            QString tag(tagChar);
            tag.resize(tagSize);
            QStringList tagList=tag.split('=');
            if(tagList.count()==2){
                QPair <QString,QString> dupa(tagList.at(0),tagList.at(1));
                tags.append(dupa);
            }
        }
        Poi poi(lon,lat,name,0,tags);
        ret.append(poi);
    }



    return ret;
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


        //FOR INDEX
        QFile qfileIndex(file+".pidx");
        qfileIndex.open(QIODevice::WriteOnly);
        QDataStream outIndex(&qfileIndex);
        //FOR DATA
        QFile qfileData(file+".ppoi");
        qfileData.open(QIODevice::WriteOnly);
        QDataStream outData(&qfileData);


        //------------Data Types-----------
        QList<int> typesList=types.keys();
        qSort(typesList);
        //Types count
        int typesC=typesList.size();
        outData<<(quint8)typesList.size();              //fileDATA     1byte(n)
        QListIterator<int> typesListIter(typesList);        
        while(typesListIter.hasNext()){            
            QString s=types.value(typesListIter.next());
            qDebug()<<s.length()<<s;
            outData<<(quint8)s.length();                 //fileDATA     1byte(m)*n
            outData.writeRawData(s.toLatin1(),s.length());//fileDATA     1byte*m
        }


        //------------Index init-----------
        QListIterator<Geohash> listIterator(geokeys);
        short number=0;
        Geohash geo1(0),geo2(0);
        unsigned int empty=0;
        while(listIterator.hasNext()){            
            if(number==0){
                geo2=listIterator.next();
                QString n=geo2.toQString();
                //geohash size
                outIndex << (quint8)(n.length());

                //geohash size
                Geohash last=geokeys.at(geokeys.size()-1);
                quint64 l=last-geo2;
                outIndex << l;
                //First geohash
                outIndex << n.toAscii();


                //Data (first block)
                outIndex<< outData.device()->pos() ;
                makeBlock(outData,geoHashedPOIs.value(geo2),typesC);


                number++;
            }
            geo1=geo2;
            geo2=listIterator.next();
            for(;geo1<geo2;geo1++){
                outIndex << empty;
            }
            outIndex << outData.device()->pos();
            makeBlock(outData,geoHashedPOIs.value(geo2),typesC);

        }
        qDebug()<<"File data size:"<<outData.device()->size()<<" Index:"<<outIndex.device()->size();

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

void PoiFilePPOI::makeBlock(QDataStream &stream,QVector<Poi*>*data,int types){
    if(data==0)
        return ;

    QBuffer poisData;
    poisData.open(QBuffer::ReadWrite);
    QDataStream out(&poisData);


    //---------Convert POI to table[type]=QVector<Poi*>*
    QVector<Poi*>*  table[types];
    for(int i=0;i<types;i++){
        table[i]=new QVector<Poi*>();
    }

    for(int i=0;i<data->size();i++){
        Poi *poi=data->at(i);
        //qDebug()<<"CoNVER"<<poi->getLat();
        table[poi->getType()]->push_back(poi);
    }
    //---------Export data
    quint16 empty=0;

    for(int i=0;i<types;i++){
        //POIs in type
        if(table[i]->size()>0){
            quint16 In=types*sizeof(short)+poisData.size();
            stream<<In;
            //POI in type
            quint16 poisCC=table[i]->size();
            //qDebug()<<poisCC;
            out<<poisCC;
            for(int j=0;j<table[i]->size();j++){
                Poi *poi=table[i]->at(j);                
                //Add LOC
                out<<(poi->getLon());
                out<<(poi->getLat());
                //qDebug()<<"LON"<<poi->getLon();

                //Add Name
                QString name=poi->getName();
                out<<((quint8)name.length());
                out.writeRawData(name.toAscii(),name.length());

                //Add Tags count
                QList <QPair<QString,QString > > list=poi->getTags();                
                out<<((quint8)list.count());

                //Add Tags
                QListIterator <QPair<QString,QString > > tagsIter(list);
                while(tagsIter.hasNext()){
                    QPair<QString,QString> pair=tagsIter.next();
                    QString valueP=pair.first+"="+pair.second;
                    out<<((quint8)valueP.length());
                    QByteArray aZ=valueP.toAscii();
                    out.writeRawData(aZ.data(),aZ.size());
                }
            }
        }else{
            //empty index
            stream<<empty;
        }
    }

    //Clear
    for(int i=0;i<types;i++){
        delete table[i];
    }

    out.device()->seek(0);
    char n[out.device()->size()];
    out.readRawData(n,out.device()->size());
    stream.writeRawData(n,out.device()->size());
}

#endif

}
