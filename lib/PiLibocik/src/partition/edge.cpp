#include <pilibocik/partition/edge.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{

Edge::Edge(){}

Edge::Edge(qint64 second,double prio,PartitionFile *part):second(second),prio(prio),part(part){
}

qint64 Edge::getPair(){
    return second;
}

Node Edge::getPairObj(){
    Node n=part->getNodeFile()->getNode(second);
    return n;
}

double Edge::getPrioritet(){
    return prio;
}


}}
