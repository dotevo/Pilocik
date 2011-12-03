#ifndef PILIBOCIK_NODE_H
#define PILIBOCIK_NODE_H

#include <QVector>

#include <pilibocik/position.h>
#include <pilibocik/partition/boundaryedge.h>
#include <pilibocik/partition/edge.h>

namespace PiLibocik{namespace Partition{
class Way;
class PartitionFile;


class Node: public Position{
private:
    int id;
    quint32 cell;

protected:
    QVector<qint64> ways;
    QVector<BoundaryEdge> boundaryEdges;
    QVector<Edge> routingEdges;
    PartitionFile *part;

public:
    Node();
    Node(int id,quint32 cell,double lon,double lat,PartitionFile *m=0);
    int getId();
    quint32 getCell();
    bool isEmpty();
    PartitionFile *getPartitionFile();
    QVector<qint64> getWays();
    void addWay(qint64 pos);
    QVector<Way> getWaysObj();
    void addBoundaryEdge(BoundaryEdge e);
    QVector<BoundaryEdge> getBoundaryEdges();
    void addRoutingEdge(Edge e);
    QVector<Edge> getRoutingEdges();
    //Node &operator=(Node &n);
};

}}

#endif // NODE_H
