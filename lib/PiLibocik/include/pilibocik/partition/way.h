#ifndef PILIBOCIK_WAY_H
#define PILIBOCIK_WAY_H

#include <QVector>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class Node;
class Restriction;

/**
 * @brief
 *
 * @class Way way.h "include/pilibocik/partition/way.h"
 */
class Way{
private:
    int id; /**< TODO */
    double prio; /**< TODO */
    QVector<qint64> nodes; /**< TODO */
    QVector<Restriction>* restriction; /**< TODO */
    qint8 oneway; /**< TODO */
    PartitionFile* part; /**< TODO */
public:
/**
 * @brief
 *
 * @fn Way
 */
    Way();
/**
 * @brief
 *
 * @fn Way
 * @param id
 * @param prio
 * @param oneway
 * @param p
 */
    Way(int id,double prio,qint8 oneway,PartitionFile*p=0);
/**
 * @brief
 *
 * @fn Way
 * @param way
 */
    Way(const Way &way);
    /**
     * @brief
     *
     * @fn ~Way
     */
    ~Way();
    /**
     * @brief
     *
     * @fn isEmpty
     */
    bool isEmpty();
    /**
     * @brief
     *
     * @fn getId
     */
    int getId();
    /**
     * @brief
     *
     * @fn getPrioritet
     */
    double getPrioritet();
    /**
     * @brief
     *
     * @fn addNode
     * @param node
     */
    void addNode(qint64 node);
    /**
     * @brief
     *
     * @fn getNodes
     * @return all nodes Inx pos in way
     */
    QVector<qint64> getNodes();
    /**
     * @brief
     *
     * @fn getNodesObj
     * @return all nodes objects in way
     */
    QVector<Node> getNodesObj();
    /**
     * @brief
     *
     * @fn addRestriction
     * @param r
     */
    void addRestriction(Restriction r);
    /**
     * @brief
     *
     * @fn getRestrictions
     */
    QVector<Restriction> getRestrictions();
    /**
     * @brief return oneway value
     *
     * @fn getOneway
     * @return return value 0-way twoway;1-direction with nodes;-1-the opposite direction
     */
    qint8 getOneway();

};

}}

#endif // WAY_H
