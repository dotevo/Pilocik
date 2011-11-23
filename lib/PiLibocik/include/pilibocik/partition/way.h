#ifndef PILIBOCIK_WAY_H
#define PILIBOCIK_WAY_H

#include <QVector>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class Node;
class Restriction;

class Way{
private:
    int id;
    double prio;
    QVector<qint64> nodes;
    QVector<Restriction>* restriction;
    qint8 oneway;
    PartitionFile* part;
public:
    Way();
    Way(int id,double prio,qint8 oneway,PartitionFile*p=0);
    Way(const Way &way);
    ~Way();
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
