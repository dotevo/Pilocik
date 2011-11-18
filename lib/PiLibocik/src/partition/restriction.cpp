#include <pilibocik\partition\restriction.h>

namespace PiLibocik{namespace Partition{

Restriction::Restriction():to(0),via(0),type(0){
}

Restriction::Restriction(quint64 to, quint64 via, int type):to(to),via(via),type(type){
}

int Restriction::getType(){
    return type;
}

quint64 Restriction::getWayTo(){
    return to;
}

quint64 Restriction::getVia(){
    return via;
}

Way Restriction::getWayToObj(){
    //TODO
    Way w;
    return w;
}

}}
