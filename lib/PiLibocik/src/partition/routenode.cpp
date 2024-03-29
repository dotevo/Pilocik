#include <pilibocik/partition/routenode.h>
#include <pilibocik/partition/partitionfile.h>

namespace PiLibocik{namespace Partition{

RouteNode::RouteNode() : Node(){}

RouteNode::RouteNode(int id, quint32 cell, double lon, double lat, int prevWayId, double rating, double distanceFromStart, RouteNode *prevNode, bool routing, PartitionFile *pf) :
                                                    Node(id, cell, lon, lat, pf),
                                                                                prevWayId(prevWayId),
                                                                                rating(rating),
                                                                                distanceFromStart(distanceFromStart),
                                                                                routing(routing),
                                                                                prevNode(prevNode){
}

RouteNode::RouteNode(Node node, int prevWayId, double rating, double distanceFromStart, RouteNode *prevNode, bool routing, PartitionFile *pf) :
                                                    Node(node.getId(), node.getCell(), node.getLon(), node.getLat(), pf),
                                                                                prevWayId(prevWayId),
                                                                                rating(rating),
                                                                                distanceFromStart(distanceFromStart),
                                                                                routing(routing),
                                                                                prevNode(prevNode) {
    ways = node.getWays();
    boundaryEdges = node.getBoundaryEdges();
    routingEdges = node.getRoutingEdges();
    part = node.getPartitionFile();
}

RouteNode *RouteNode::getPrevNode(){
    return prevNode;
}

int RouteNode::getPrevWay(){
    return prevWayId;
}

double RouteNode::getRating(){
    return rating;
}

double RouteNode::getDistanceFromStart(){
    return distanceFromStart;
}

bool RouteNode::getRouting(){
    return routing;
}

}}
