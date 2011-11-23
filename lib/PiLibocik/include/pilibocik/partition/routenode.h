#ifndef ROUTENODE_H
#define ROUTENODE_H

#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class RouteNode : public PiLibocik::Partition::Node{
private:
    RouteNode *prevNode;
    int prevWayId;
    double rating;
    bool routing;
public:
    RouteNode();
    RouteNode(int id, quint32 cell, double lon, double lat, int prevWayId, double rating, bool routing = false, RouteNode *prevNode=0, PartitionFile *m=0);
    RouteNode(Node node, int prevWayId, double rating, bool routing = false, RouteNode *prevNode=0, PartitionFile *m=0);
    RouteNode getPrevNode();
    int getPrevWay();
    double getRating();
};

}}

#endif // ROUTENODE_H
