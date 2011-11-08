#ifndef PREPAREDATA_H
#define PREPAREDATA_H

#include <QList>
#include <QMap>
#include <QMapIterator>
#include <QPair>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <pilibocik/point.h>
#include <pilibocik/poi.h>
#include <pilibocik/geohash.h>

namespace PiLibocik{

    class PrepareData
    {
    private:
        QSqlDatabase db;
        QStringList queriesBuffer;
        QList<Poi> poiList;
        QMap<int,QPair<QString,QString> > poiTypes;
        QMap<int,QString> poiTypeNames;
        QMap<int,QList<QString> > poiTypeSubtags;
        QMap<int,QList<Point> > wayNodes;

        void loadXMLconfig(QString XMLpath);
        void generateData();
        Point shapeToPoint(QList<Point> shape);
        //TODO
        //bool createTables();

    public:
        PrepareData(QString dbPath, QString XMLconfigPath);

        QList<Poi> getPoiList();
        QMap<int,QString> getPoiTypeNames();

    };
}

#endif // PREPAREDATA_H
