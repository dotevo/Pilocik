#ifndef POI_H
#define POI_H

#include <QString>
#include <QList>
#include <QPair>
#include <QMap>
#include <pilibocik/point.h>

namespace PiLibocik{

    /**
     * @brief
     *
     * @class Poi poi.h <pilibocik/poi.h>
     */
    class Poi:public Point
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
