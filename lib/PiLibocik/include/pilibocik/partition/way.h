#ifndef WAY_H
#define WAY_H

#include <QVector>
#include <pilibocik/partition/restriction.h>
#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{
class Node;
class Restriction;

class Way{
private:
    int id;
    double prio;
    QVector<qint64> nodes;
    QVector<Restriction> restriction;
    qint8 oneway;
public:
    Way();
    Way(int id,double prio,qint8 oneway);
    bool isEmpty();
    int getId();
    double getPrioritet();
    void addNode(qint64 node);
    QVector<qint64> getNodes();
    QVector<Node> getNodesObj();
    void addRestriction(Restriction r);
    QVector<Restriction> getRestrictions();
    qint8 getOneway();

};

}}

#endif // WAY_H
