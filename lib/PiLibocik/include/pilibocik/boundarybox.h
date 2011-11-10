#ifndef BOUNDARYBOX_H
#define BOUNDARYBOX_H

#include <pilibocik/point.h>
#include <pilibocik/geohash.h>
#include <QList>

namespace PiLibocik{


class BoundaryBox{
private:
    Point p1;
    Point p2;
public:
    BoundaryBox(Point p1,Point p2);
    QList <Geohash> getGeohashesIn(int precision);
};

}

#endif // BOUNDARYBOX_H
