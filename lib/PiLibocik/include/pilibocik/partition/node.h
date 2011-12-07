#ifndef PILIBOCIK_NODE_H
#define PILIBOCIK_NODE_H

#include <QVector>

#include <pilibocik/position.h>
#include <pilibocik/partition/boundaryedge.h>
#include <pilibocik/partition/edge.h>

namespace PiLibocik{namespace Partition{
class Way;
class PartitionFile;


/**
 * @brief Represent partition node.
 *
 * @class Node node.h "include/pilibocik/partition/node.h"
 */
class Node: public Position{
private:
    int id; /** Node id */
    quint32 cell; /** Cell in partition */

protected:
    QVector<qint64> ways; /** Ways */
    QVector<BoundaryEdge> boundaryEdges; /** Boundary Edges */
    QVector<Edge> routingEdges; /** Routing Edges */
    PartitionFile *part; /** File support */

public:
/**
 * @brief Empty constructor
 *
 * @fn Node
 */
    Node();
/**
 * @brief
 *
 * @fn Node
 * @param id Node id
 * @param cell Cell in partition
 * @param lon Lon position
 * @param lat Lat position
 * @param m Needed when data is from file
 */
    Node(int id,quint32 cell,double lon,double lat,PartitionFile *m=0);
    /**
     * @brief Return id of node
     *
     * @fn getId
     */
    int getId();
    /**
     * @brief Return node cell number in partition
     *
     * @fn getCell
     */
    quint32 getCell();
    /**
     * @brief If node's made by empty constructor return true
     *
     * @fn isEmpty
     */
    bool isEmpty();
    /**
     * @brief Return PartitionFile
     *
     * @fn getPartitionFile
     */
    PartitionFile *getPartitionFile();
    /**
     * @brief Get ways indexes
     *
     * @fn getWays
     */
    QVector<qint64> getWays();
    /**
     * @brief Add way to node.
     *
     * @fn addWay
     * @param pos When you load file pos mean index in ways file.
     */
    void addWay(qint64 pos);
    /**
     * @brief Get ways objects ( Slower then getWays() )
     *
     * @fn getWaysObj
     */
    QVector<Way> getWaysObj();
    /**
     * @brief Add boundary edge to node ( it's connection with another node )
     *
     * @fn addBoundaryEdge
     * @param e
     */
    void addBoundaryEdge(BoundaryEdge e);
    /**
     * @brief Get boundary edges.
     *
     * @fn getBoundaryEdges
     */
    QVector<BoundaryEdge> getBoundaryEdges();
    /**
     * @brief Add routing edge to node.
     *
     * @fn addRoutingEdge
     * @param e
     */
    void addRoutingEdge(Edge e);
    /**
     * @brief Get routing edges.
     *
     * @fn getRoutingEdges
     */
    QVector<Edge> getRoutingEdges();
    //Node &operator=(Node &n);
};

}}

#endif // NODE_H
