#ifndef PREPAREDATA_H
#define PREPAREDATA_H

#include <QList>
#include <QMap>
#include <QPair>
#include <pilibocik/point.h>
#include <pilibocik/poi.h>

namespace PiLibocik{

    class PrepareData
    {
    private:
        QList<Poi> poiList;
        QMap<int,QPair<QString,QString> > poiTypes;
        QMap<int,QList<QString> > poiTypeSubtags;

    public:
        PrepareData(QString dbPath);
        bool openDb();
        //TODO
        //bool createTables();
        void generateData();
        QList<Poi> getPoiList();
        Point shapeToPoint(QList<Point>);
    };
}

#endif // PREPAREDATA_H
