#ifndef POINT_H
#define POINT_H

namespace PiLibocik{
    class Point
    {
    private:
        double lon,
               lat;
    public:
        Point(double lon, double lat);

        double getLon();
        double getLat();
    };
}

#endif // POINT_H
