#ifndef PILIBOCIK_POI_H
#define PILIBOCIK_POI_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QPair>
#include <QMap>
#include <QDate>
#include <pilibocik/position.h>

namespace PiLibocik{

    /**
     * @brief
     *
     * @class Poi poi.h <pilibocik/poi.h>
     */
    class Poi:public Position
    {
    private:
        QMap <int,QString> types; /**< TODO */

        QString         name; /**< TODO */
        int             type; /**< TODO */
        QList < QPair <QString, QString> >    tags; /**< TODO */
//#ifdef PiLibocik_WRITE_MODE
        QString         geohash; /**< TODO */
//#endif
    public:
/**
 * @brief
 *
 * @fn Poi
 * @param lon
 * @param lat
 * @param name
 * @param type
 * @param QList<QPair<QString
 * @param tags
 */
        Poi(double lon,double lat,QString name, int type, QList < QPair <QString, QString> > tags);
        /**
         * @brief
         *
         * @fn getName
         * @return QString with name of POI
         */
        QString getName() const;
        /**
         * @brief
         *
         * @fn getTags
         * @return all tags at POI
         */
        QList < QPair <QString, QString> > getTags() const;
        /**
         * @brief
         *
         * @fn getType
         * @return type Id
         */
        int getType() const;
        /**
         * @brief
         *
         * @fn setTypes
         * @param QMap<int
         * @param types
         */
        void setTypes(QMap <int,QString> types);
        /**
         * @brief
         *
         * @fn getTypeName
         * @param type
         */
        QString getTypeName(int type);
        /**
         * @brief
         *
         * @param now
         */
        int isOpen(QDate now);


//#ifdef PiLibocik_WRITE_MODE
/**
 * @brief
 *
 * @fn Poi
 * @param lon
 * @param lat
 * @param name
 * @param type
 * @param QList<QPair<QString
 * @param tags
 * @param geohash
 */
        Poi(double lon,double lat,QString name, int type, QList < QPair <QString, QString> > tags, QString geohash);
        /**
         * @brief
         *
         * @fn getGeohash
         */
        QString getGeohash();
//#endif
    };
}

#endif // POI_H
