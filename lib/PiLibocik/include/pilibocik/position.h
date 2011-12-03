#ifndef LOCATION_H
#define LOCATION_H

#include <pilibocik/geohash.h>

namespace PiLibocik{
    /**
     * @brief
     *
     * @class Position position.h <pilibocik/position.h>
     */
    class Position
    {
    protected:
        double lon, /**< TODO */
               lat; /**< TODO */

    public:
        /**
         * @brief
         *
         */
        Position();
        /**
         * @brief
         *
         * @fn Position
         * @param lon
         * @param lat
         */
        Position(double lon, double lat);


        /**
         * @brief
         *
         * @fn getLon
         */
        double getLon();
        /**
         * @brief
         *
         * @param lon
         */
        void setLon(double lon);
        /**
         * @brief
         *
         * @fn getLat
         */
        double getLat();
        /**
         * @brief
         *
         * @param lat
         */
        void setLat(double lat);
        /**
         * @brief
         *
         * @fn getGeohash
         */        
        Geohash getGeohash(int p);
        /**
         * @brief
         *
         * @fn getDistance
         * @return distance
         */
        double getDistance(Position p);
        /**
         * @brief (self including)
         *
         * @param pair
         *
         * @return
         */
        QList< Position > getPositionsAround(QPair< double, double > pair);
    };
}

#endif // LOCATION_H
