#include <pilibocik\partition\edge.h>

namespace PiLibocik{namespace Partition{

Edge::Edge(){}

Edge::Edge(qint64 second,double prio):second(second),prio(prio){
}

qint64 Edge::getPair(){
    return second;
}

Node Edge::getPairObj(){
    //TODO
    Node n;
    return n;
}

double Edge::getPrioritet(){
    return prio;
}

}}
