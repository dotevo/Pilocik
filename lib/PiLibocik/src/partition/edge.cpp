#include <pilibocik\partition\edge.h>

namespace PiLibocik{namespace Partition{

Edge::Edge(){
}

quint64 Edge::getPair(){
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
