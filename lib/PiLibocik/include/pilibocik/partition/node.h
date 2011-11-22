#ifndef NODE_H
#define NODE_H

#include <QVector>
#include <pilibocik/position.h>
#include <pilibocik/partition/edge.h>
#include <pilibocik/partition/way.h>

namespace PiLibocik{namespace Partition{
class Edge;
class Way;


class Node: public Position{
private:
    int id;
    QVector<qint64> ways;
    QVector<Edge> boundaryEdges;
    QVector<Edge> routingEdges;
public:
    Node();
    Node(int id,double lon,double lat);
    int getId();
    bool isEmpty();
    QVector<qint64> getWays();
    void addWay(qint64 pos);
    QVector<Way> getWaysObj();
    void addBoundaryEdge(Edge e);
    QVector<Edge> getBoundaryEdges();
    void addRoutingEdge(Edge e);
    QVector<Edge> getRoutingEdges();
};

}}

#endif // NODE_H
