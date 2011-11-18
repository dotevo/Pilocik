#include <pilibocik\partition\node.h>
#include <pilibocik/partition/partitionfile.h>
#include <QDebug>

namespace PiLibocik{namespace Partition{

Node::Node():Point(0,0){
    id=-1;
}

Node::Node(int id,double lon,double lat,PartitionFile*p):Point(lon,lat),part(p){
    this->id=id;
}

bool Node::isEmpty(){
    return id<0;
}

int Node::getId(){
    return id;
}

void Node::addWay(qint64 pos){
    ways.push_back(pos);
}

QVector<qint64> Node::getWays(){
    return ways;
}

QVector<Way> Node::getWaysObj(){
    QVector<Way> waysObj;
    for(int i=0;i<ways.size();i++){
        qint64 a=ways.at(i);
        waysObj.push_back(part->getWayFile()->getWay(a));
    }
    return waysObj;
}

void Node::addBoundaryEdge(Edge e){
    boundaryEdges.push_back(e);
}

QVector<Edge> Node::getBoundaryEdges(){
    return boundaryEdges;
}

void Node::addRoutingEdge(Edge e){
    routingEdges.push_back(e);
}

QVector<Edge> Node::getRoutingEdges(){
    return routingEdges;
}


}}
