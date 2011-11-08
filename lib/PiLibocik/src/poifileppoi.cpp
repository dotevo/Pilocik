#include <pilibocik/poifileppoi.h>

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
    //TODO SAVE DATA
    QDataStream out(&file);
    //Magic number to know file TYPE
    out << (quint32)0xABCDCCDD;
    file.close();
}
#endif

}
