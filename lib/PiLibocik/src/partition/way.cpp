#include <pilibocik\partition\way.h>

namespace PiLibocik{namespace Partition{

Way::Way(){
}

Way::Way(int id,qint8 oneway){
}

bool Way::isEmpty(){
    return id<0;
}

int Way::getId(){
    return id;
}


void Way::addNode(qint64 node){
    nodes.append(node);
}

QVector<qint64> Way::getNodes(){
    return nodes;
}

QVector<Node> Way::getNodesObj(){
    //TODO
    QVector<Node> n;
    return n;
}

void Way::addRestriction(Restriction r){
    restriction.append(r);
}

QVector<Restriction> Way::getRestrictions(){
    //TODO
    QVector<Restriction> r;
    return r;
}

qint8 Way::getOneway(){
    return oneway;
}

}}
