#ifndef PILIBOCIK_ROUTENODE_H
#define PILIBOCIK_ROUTENODE_H

#include <pilibocik/partition/node.h>

namespace PiLibocik{namespace Partition{
class PartitionFile;
class RouteNode : public PiLibocik::Partition::Node{
private:
    RouteNode *prevNode;
    int prevWayId;
    double rating;
    bool routing;
    double distanceFromStart;
public:
    /**
     * @brief Default constructor
     *
     */
    RouteNode();
    /**
     * @brief Constructor
     *
     * @param id
     * @param cell
     * @param lon
     * @param lat
     * @param prevWayId - id of the way through which came
     * @param rating
     * @param prevNode - pointer to node from which came
     * @param routing
     * @param pf - pointer to partition file
     */
    RouteNode(int id, quint32 cell, double lon, double lat, int prevWayId, double rating, double distanceFromStart=0, RouteNode *prevNode=0, bool routing = false, PartitionFile *pf=0);
    /**
     * @brief Constructor
     *
     * @param node
     * @param prevWayId - id of the way through which came
     * @param rating
     * @param prevNode - pointer to node from which came
     * @param routing
     * @param pf - pointer to partition file
     */
    RouteNode(Node node, int prevWayId, double rating, double distanceFromStart=0, RouteNode *prevNode=0, bool routing = false, PartitionFile *pf=0);
    /**
     * @brief
     *
     * @return node object
     */
    RouteNode getPrevNode();
    /**
     * @brief
     *
     * @return id of way
     */
    int getPrevWay();
    /**
     * @brief
     *
     * @return value of rating
     */
    double getRating();
    /**
     * @brief
     *
     */
    double getDistanceFromStart();
    /**
     * @brief Gets boolean value that indicates if it's routing edge
     *
     * @return true - is routing edge | false - is not
     */
    bool getRouting();
};

}}

#endif // ROUTENODE_H
