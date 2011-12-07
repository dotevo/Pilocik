#ifndef PILIBOCIK_RESTRICTION_H
#define PILIBOCIK_RESTRICTION_H

#include <qglobal.h>

namespace PiLibocik{namespace Partition{
class Way;
class Node;

/**
 * @brief Way restriction class.
 *
 * Restriction like "don't turn left"
 *
 * @class Restriction restriction.h "include/pilibocik/partition/restriction.h"
 */
class Restriction{
private:
    qint64 to; /** way */
    qint64 via; /** node */
    int type; /** type */
public:
/**
 * @brief Empty constructor
 *
 * @fn Restriction
 */
    Restriction();
/**
 * @brief
 *
 * @fn Restriction
 * @param to Way index
 * @param via Node index
 * @param type Type of restriction TODO
 */
    Restriction(qint64 to, qint64 via, int type);
    /**
     * @brief Return type of restriction.
     *
     * @fn getType
     */
    int getType();
    /**
     * @brief Get way 'to' index.
     *
     * @fn getWayTo
     */
    quint64 getWayTo();
    /**
     * @brief Get node 'via' index.
     *
     * @fn getVia
     */
    quint64 getVia();
    /**
     * @brief Get way 'to' object.
     *
     * @fn getWayToObj
     */
    Way getWayToObj();
    /**
     * @brief Get node 'via' object.
     *
     * @fn getViaObj
     */
    Node getViaObj();
};

}}


#endif // RESTRICTION_H
