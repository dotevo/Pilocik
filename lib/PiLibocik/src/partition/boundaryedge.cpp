#include <pilibocik/partition/boundaryedge.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/way.h>

namespace PiLibocik{namespace Partition{


BoundaryEdge::BoundaryEdge():Edge(){}

BoundaryEdge::BoundaryEdge(qint64 second,qint64 way,double prio,PartitionFile *part):
                                                    Edge(second,prio,part),way(way){
}

qint64 BoundaryEdge::getWay(){
    return way;
}

Way BoundaryEdge::getWayObj(){
    Way w=part->getWayFile()->getWay(way);
    return w;
}

}}

