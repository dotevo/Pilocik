#ifndef POITYPES_H
#define POITYPES_H

#include <QMap>

namespace PiLibocik {
    class PoiTypes
    {
    private:
        QMap<int, QString> poiTypes;
        QMap<int, QString> poiTypeSubtags;

        processXML(QString XMLpath);
    public:
        PoiTypes(QString poiConfigXMLpath);
        getTypeName(int id);
    };
}

#endif // POITYPES_H
