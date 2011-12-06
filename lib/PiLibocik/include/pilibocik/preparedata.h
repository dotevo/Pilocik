#ifndef PILIBOCIK_PREPAREDATA_H
#define PILIBOCIK_PREPAREDATA_H

#ifdef PiLibocik_WRITE_MODE

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

    class PrepareData: public QObject
    {
        Q_OBJECT
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
        PrepareData();

        QList<Poi> getPoiList();
        QMap<int,QString> getPoiTypeNames();

        void init(QString dbMapPath, QString XMLconfigPath);
        void generateData();
        void saveToDatabase();
        void loadXMLconfig(QString XMLpath);
        void loadFromDatabase(QString dbLoadPath);

    signals:
        void progress(int percent);

    };
}
#endif
#endif // PREPAREDATA_H
