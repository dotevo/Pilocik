#ifndef LOCATION_H
#define LOCATION_H

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
    };
}

#endif // LOCATION_H
