#ifndef PILIBOCIK_EDGE_H
#define PILIBOCIK_EDGE_H

#include <QtGlobal>

namespace PiLibocik{namespace Partition{
class Node;
class PartitionFile;

class Edge{
private:
    qint64 second;
    double prio;
protected:
    PartitionFile *part;
public:
    Edge();
    Edge(qint64 second,double prio,PartitionFile *m=0);
    qint64 getPair();
    Node getPairObj();
    double getPrioritet();
};




}}

#endif // EDGE_H
