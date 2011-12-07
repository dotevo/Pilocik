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
     * @class PoiDisplay poi.h "include/pilibocik/poi.h"
     */
    class PoiDisplay
    {
    private:
        int     type; /**< TODO */
        bool    display; /**< TODO */
        int     zoom; /**< TODO */
        QString iconPath; /**< TODO */

    public:
/**
 * @brief
 *
 * @fn PoiDisplay
 */
        PoiDisplay();
/**
 * @brief
 *
 * @fn PoiDisplay
 * @param type
 * @param display
 * @param zoom
 * @param iconPath
 */
        PoiDisplay(int type, bool display, int zoom, QString iconPath);
        /**
         * @brief
         *
         * @fn setDisplay
         * @param display
         */
        void setDisplay(bool display);
        /**
         * @brief
         *
         * @fn getDisplay
         */
        bool getDisplay();
        /**
         * @brief
         *
         * @fn getZoom
         */
        int  getZoom();
        /**
         * @brief
         *
         * @fn getType
         */
        int  getType();
        /**
         * @brief
         *
         * @fn getIconPath
         */
        QString getIconPath();
        /**
         * @brief
         *
         * @fn operator ==
         * @param p
         */
        bool operator ==(const PoiDisplay &p) const;
    };


    /**
     * @brief
     *
     * @class Poi poi.h <pilibocik/poi.h>
     */
    /**
     * @brief
     *
     * @class Poi poi.h "include/pilibocik/poi.h"
     */
    class Poi:public Position
    {
    private:
        QMap <int,QString> types; /**< TODO */ /**< TODO */

        QString         name; /**< TODO */ /**< TODO */
        int             type; /**< TODO */ /**< TODO */
        QList < QPair <QString, QString> >    tags; /**< TODO */ /**< TODO */
//#ifdef PiLibocik_WRITE_MODE
        QString         geohash; /**< TODO */ /**< TODO */
//#endif
    public:
        /**
         * @brief
         *
         */
/**
 * @brief
 *
 * @fn Poi
 */
        Poi();
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
        /**
         * @brief
         *
         * @fn getName
         */
        QString getName() const;
        /**
         * @brief
         *
         * @fn getTags
         * @return all tags at POI
         */
        /**
         * @brief
         *
         * @fn getTags
         */
        QList < QPair <QString, QString> > getTags() const;
        /**
         * @brief
         *
         * @fn getType
         * @return type Id
         */
        /**
         * @brief
         *
         * @fn getType
         */
        int getType() const;
        /**
         * @brief
         *
         * @fn setTypes
         * @param QMap<int
         * @param types
         */
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
        /**
         * @brief
         *
         * @fn isOpen
         * @param now
         */
        int isOpen(QDateTime now);


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
