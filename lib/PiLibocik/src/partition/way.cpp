#include <pilibocik/partition/way.h>
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/restriction.h>
#include <QDebug>

namespace PiLibocik{namespace Partition{

Way::Way(){
    restriction=new QVector<Restriction>();
}

Way::Way(const Way &way){
    id=way.id;
    prio=way.prio;
    nodes=way.nodes;
    oneway=way.oneway;
    part=way.part;
    restriction=new QVector<Restriction>();
    for(int i=0;i<way.restriction->size();i++)
        restriction->push_back(way.restriction->at(i));
}

Way::Way(int id,double prio,qint8 oneway,PartitionFile*p):id(id),prio(prio),oneway(oneway),part(p){
    restriction=new QVector<Restriction>();
}

Way::~Way(){
    delete restriction;
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

double Way::getPrioritet(){
    return prio;
}

QVector<qint64> Way::getNodes(){
    return nodes;
}

QVector<Node> Way::getNodesObj(){
    QVector<Node> nodesObj;
    for(int i=0;i<nodes.size();i++){
        qint64 a=nodes.at(i);
        nodesObj.push_back(part->getNodeFile()->getNode(a));
    }
    return nodesObj;
}

void Way::addRestriction(Restriction r){
    restriction->append(r);
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
