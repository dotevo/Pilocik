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
    private:
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
        double setLon(double lon);
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
        double setLat(double lat);
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
        double getSimpleDistance(Position p);
    };
}

#endif // LOCATION_H
