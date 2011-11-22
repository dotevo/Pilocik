#ifndef PILIBOCIK_PREPAREDATA_H
#define PILIBOCIK_PREPAREDATA_H

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
#include <pilibocik/position.h>
#include <pilibocik/poi.h>
#include <pilibocik/geohash.h>

namespace PiLibocik{

    class PrepareData
    {
    private:
        QSqlDatabase db;
        QStringList queriesBuffer;
        QList<Poi> poiList;
        QList<Poi> poiFromNodesList;
        QMap<int, QList<QPair<QString,QString> > > poiTypes;
        QMap<int,QString> poiTypeNames;
        QMap<int,QList<QString> > poiTypeSubtags;
        QMap<int,QList<Position> > wayNodes;

        Position shapeToPoint(QList<Position> shape);
        void removePoiDuplicates();
        void createTables();

    public:
        PrepareData(QString dbMapPath, QString dbOutPath, QString XMLconfigPath);
        PrepareData(QString dbMapPath, QString XMLconfigPath);
        PrepareData(QString dbLoadPath);

        QList<Poi> getPoiList();
        QMap<int,QString> getPoiTypeNames();

        void generateData();
        void saveToDatabase();
        void loadXMLconfig(QString XMLpath);
        void loadFromDatabase(QString dbLoadPath);

    };
}

#endif // PREPAREDATA_H
