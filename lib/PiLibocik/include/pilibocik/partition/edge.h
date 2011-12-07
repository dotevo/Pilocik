#ifndef PILIBOCIK_EDGE_H
#define PILIBOCIK_EDGE_H

#include <QtGlobal>

namespace PiLibocik{namespace Partition{
class Node;
class PartitionFile;

/**
 * @brief Class represents Edge. Node with priority.
 *
 * @class Edge edge.h "include/pilibocik/partition/edge.h"
 */
class Edge{
private:
    qint64 second; /** Second node. */
    double prio; /** Priority */
protected:
    PartitionFile *part; /** File support */
public:
/**
 * @brief Empty constructor
 *
 * @fn Edge
 */
    Edge();
/**
 * @brief Constructor
 *
 * @fn Edge
 * @param second Node index
 * @param prio Priority of edge
 * @param m Needed only in loaded file
 */
    Edge(qint64 second,double prio,PartitionFile *m=0);
    /**
     * @brief Return node index in pair
     *
     * @fn getPair
     */
    qint64 getPair();
    /**
     * @brief Return node object only when you loaded file.
     *
     * @fn getPairObj
     */
    Node getPairObj();
    /**
     * @brief Return priority of connection
     *
     * @fn getPrioritet
     */
    double getPrioritet();
};




}}

#endif // EDGE_H
