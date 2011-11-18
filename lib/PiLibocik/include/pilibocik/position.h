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
         * @fn getLat
         */
        double getLat();
        /**
         * @brief
         *
         * @fn getGeohash
         */
        Geohash getGeohash(int p);
    };
}

#endif // LOCATION_H
