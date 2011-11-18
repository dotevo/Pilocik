#ifndef PILIBOCIK_POINT_H
#define PILIBOCIK_POINT_H

#include <pilibocik/geohash.h>

namespace PiLibocik{
    /**
     * @brief
     *
     * @class Point point.h <pilibocik/point.h>
     */
    class Point
    {
    private:
        double lon, /**< TODO */
               lat; /**< TODO */
    public:
        /**
          * @brief
          *
          * @fn Point
          * @param lon
          * @param lat
          */
        Point(double lon, double lat);

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

#endif // POINT_H
