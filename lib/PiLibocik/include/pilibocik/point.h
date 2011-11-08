#ifndef POINT_H
#define POINT_H

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
    };
}

#endif // POINT_H
