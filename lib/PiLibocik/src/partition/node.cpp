#include <pilibocik/partition/node.h>
#include <pilibocik/partition/way.h>
#include <pilibocik/partition/partitionfile.h>

#include <QDebug>

namespace PiLibocik{namespace Partition{

Node::Node():Position(0,0){
    id=-1;
    cell=0;
}

Node::Node(int id,quint32 cell,double lon,double lat,PartitionFile *m):Position(lon,lat),cell(cell),part(m){
    this->id=id;
}

bool Node::isEmpty(){
    return id<0;
}

PartitionFile *Node::getPartitionFile(){
    return part;
}

quint32 Node::getCell(){
    return cell;
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

void Node::addBoundaryEdge(BoundaryEdge e){
    boundaryEdges.push_back(e);
}

QVector<BoundaryEdge> Node::getBoundaryEdges(){
    return boundaryEdges;
}

void Node::addRoutingEdge(Edge e){
    routingEdges.push_back(e);
}

QVector<Edge> Node::getRoutingEdges(){
    return routingEdges;
}


}}
