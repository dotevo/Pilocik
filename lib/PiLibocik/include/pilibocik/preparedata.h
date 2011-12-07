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

    /**
     * @brief
     *
     * @class PrepareData preparedata.h "include/pilibocik/preparedata.h"
     */
    class PrepareData: public QObject
    {
        Q_OBJECT
    private:
        QSqlDatabase db; /**< TODO */
        QStringList queriesBuffer; /**< TODO */
        QList<Poi> poiList; /**< TODO */
        QList<Poi> poiFromNodesList; /**< TODO */
        QMap<int, QList<QPair<QString,QString> > > poiTypes; /**< TODO */
        QMap<int,QString> poiTypeNames; /**< TODO */
        QMap<int,QList<QString> > poiTypeSubtags; /**< TODO */
        QMap<int,QList<Position> > wayNodes; /**< TODO */

        /**
         * @brief
         *
         * @fn shapeToPoint
         * @param shape
         */
        Position shapeToPoint(QList<Position> shape);
        /**
         * @brief
         *
         * @fn removePoiDuplicates
         */
        void removePoiDuplicates();
        /**
         * @brief
         *
         * @fn createTables
         */
        void createTables();

    public:
/**
 * @brief
 *
 * @fn PrepareData
 * @param dbMapPath
 * @param dbOutPath
 * @param XMLconfigPath
 */
        PrepareData(QString dbMapPath, QString dbOutPath, QString XMLconfigPath);
/**
 * @brief
 *
 * @fn PrepareData
 * @param dbMapPath
 * @param XMLconfigPath
 */
        PrepareData(QString dbMapPath, QString XMLconfigPath);
/**
 * @brief
 *
 * @fn PrepareData
 * @param dbLoadPath
 */
        PrepareData(QString dbLoadPath);
/**
 * @brief
 *
 * @fn PrepareData
 */
        PrepareData();

        /**
         * @brief
         *
         * @fn getPoiList
         */
        QList<Poi> getPoiList();
        /**
         * @brief
         *
         * @fn getPoiTypeNames
         */
        QMap<int,QString> getPoiTypeNames();

        /**
         * @brief
         *
         * @fn init
         * @param dbMapPath
         * @param XMLconfigPath
         */
        void init(QString dbMapPath, QString XMLconfigPath);
        /**
         * @brief
         *
         * @fn generateData
         */
        void generateData();
        /**
         * @brief
         *
         * @fn saveToDatabase
         */
        void saveToDatabase();
        /**
         * @brief
         *
         * @fn loadXMLconfig
         * @param XMLpath
         */
        void loadXMLconfig(QString XMLpath);
        /**
         * @brief
         *
         * @fn loadFromDatabase
         * @param dbLoadPath
         */
        void loadFromDatabase(QString dbLoadPath);

    signals:
        /**
         * @brief
         *
         * @fn progress
         * @param percent
         */
        void progress(int percent);

    };
}
#endif
#endif // PREPAREDATA_H
