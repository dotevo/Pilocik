#ifndef PILIBOCIK_BOUNDARYBOX_H
#define PILIBOCIK_BOUNDARYBOX_H

#include <pilibocik/position.h>
#include <pilibocik/geohash.h>
#include <QList>
#include <QPair>
#include <QMap>

namespace PiLibocik{


/**
 * @brief Boundary box has positions of two corners.
 *
 * @class BoundaryBox boundarybox.h "include/pilibocik/boundarybox.h"
 */
class BoundaryBox{
private:
    Position p1; /**< TODO */
    Position p2; /**< TODO */
    int precision; /**< TODO */
    QMap< int, QPair< double,double > > spatialError; /**< TODO */
public:
/**
 * @brief Constructor
 *
 * @fn BoundaryBox
 * @param p1 First corner
 * @param p2 Second corner
 */
    BoundaryBox(Position p1,Position p2);
    /**
     * @brief
     *
     * @fn getCurrentError
     * @return Error of precision
     */
    QPair< double,double > getCurrentError();
    /**
     * @brief List of geohashes in box.
     *
     * @fn getGeohashesIn
     * @param precision precision of geohash.
     */
    QList < Geohash > getGeohashesIn(int precision);
};

}

#endif // BOUNDARYBOX_H
