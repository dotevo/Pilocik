#ifndef PILIBOCIK_EDGE_H
#define PILIBOCIK_EDGE_H

#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{
class Node;

class Edge{
private:
    qint64 second;
    double prio;
public:
    Edge();
    Edge(qint64 second,double prio);
    qint64 getPair();
    Node getPairObj();
    double getPrioritet();
};


}}

#endif // EDGE_H
