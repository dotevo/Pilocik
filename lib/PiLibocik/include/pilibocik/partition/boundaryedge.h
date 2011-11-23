#ifndef PILIBOCIK_BOUNDARYEDGE_H
#define PILIBOCIK_BOUNDARYEDGE_H

#include <pilibocik/partition/edge.h>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class Way;
class BoundaryEdge:public PiLibocik::Partition::Edge{
private:
    qint64 way;
public:
    BoundaryEdge();
    BoundaryEdge(qint64 second,qint64 way,double prio,PartitionFile *m=0);
    qint64 getWay();
    Way getWayObj();
};

}}


#endif // BOUNDARYEDGE_H
