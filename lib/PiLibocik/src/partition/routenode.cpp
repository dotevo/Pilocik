#include <pilibocik/partition/routenode.h>
#include <pilibocik/partition/partitionfile.h>

namespace PiLibocik{namespace Partition{

RouteNode::RouteNode() : Node(){}

RouteNode::RouteNode(int id, quint32 cell, double lon, double lat, int prevWayId, double rating, bool routing = false, RouteNode *prevNode=0, PartitionFile *m=0) :
                                                    Node(id, cell, lon, lat, m),
                                                                                prevWayId(prevWayId),
                                                                                rating(rating),
                                                                                routing(routing),
                                                                                prevNode(prevNode){
}

RouteNode::RouteNode(Node node, int prevWayId, double rating, bool routing = false, RouteNode *prevNode=0, PartitionFile *m=0) :
                                                    Node(node.getId(), node.getCell(), node.getLon(), node.getLat(), m),
                                                                                prevWayId(prevWayId),
                                                                                rating(rating),
                                                                                routing(routing),
                                                                                prevNode(prevNode){
}

RouteNode RouteNode::getPrevNode(){
    return &prevNode;
}

int RouteNode::getPrevWay(){
    return prevWayId;
}

double RouteNode::getRating(){
    return rating;
}

}}
