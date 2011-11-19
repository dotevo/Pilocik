#ifndef PILIBOCIK_BOUNDARYBOX_H
#define PILIBOCIK_BOUNDARYBOX_H

#include <pilibocik/position.h>
#include <pilibocik/geohash.h>
#include <QList>
#include <QPair>
#include <QMap>

namespace PiLibocik{


class BoundaryBox{
private:
    Position p1;
    Position p2;
    QMap<int, QPair<double,double> > spatialError;
public:
    BoundaryBox(Position p1,Position p2);
    QList <Geohash> getGeohashesIn(int precision);
};

}

#endif // BOUNDARYBOX_H
