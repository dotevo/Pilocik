#ifndef EDGE_H
#define EDGE_H

#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{
class Node;

class Edge{
private:
    quint64 second;
    double prio;
public:
    Edge();
    quint64 getPair();
    Node getPairObj();
    double getPrioritet();
};


}}

#endif // EDGE_H
