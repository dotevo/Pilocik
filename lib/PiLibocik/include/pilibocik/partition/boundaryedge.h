#ifndef PILIBOCIK_BOUNDARYEDGE_H
#define PILIBOCIK_BOUNDARYEDGE_H

#include <pilibocik/partition/edge.h>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class Way;

/**
 * @brief Represent connection between nodes in different cell.
 *
 * @class BoundaryEdge boundaryedge.h "include/pilibocik/partition/boundaryedge.h"
 */
class BoundaryEdge:public PiLibocik::Partition::Edge{
private:
    qint64 way; /** Way index */
public:
/**
 * @brief Empty constructor
 *
 * @fn BoundaryEdge
 */
    BoundaryEdge();
/**
 * @brief Constructor with data
 *
 * @fn BoundaryEdge
 * @param second Save: index in nodes file. Load: index at list of nodes.
 * @param way Save: index in ways file. Load: index at list of ways
 * @param prio Priority of way
 * @param m no needed when you save data
 */
    BoundaryEdge(qint64 second,qint64 way,double prio,PartitionFile *m=0);
    /**
     * @brief This function return index of way between nodes.
     *
     * @fn getWay
     * @return Index of way
     */
    qint64 getWay();
    /**
     * @brief This function return way object
     *
     * This function's slower then getWay(), becouse need file reading.
     *
     * @fn getWayObj
     * @return Way object between nodes
     */
    Way getWayObj();
};

}}


#endif // BOUNDARYEDGE_H
