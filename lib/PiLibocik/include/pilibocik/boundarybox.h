#ifndef BOUNDARYBOX_H
#define BOUNDARYBOX_H

#include <pilibocik/point.h>
#include <pilibocik/geohash.h>
#include <QList>
#include <QPair>
#include <QMap>

namespace PiLibocik{


class BoundaryBox{
private:
    Point p1;
    Point p2;
    QMap<int, QPair<double,double> > spatialError;
public:
    BoundaryBox(Point p1,Point p2);
    QList <Geohash> getGeohashesIn(int precision);
};

}

#endif // BOUNDARYBOX_H
