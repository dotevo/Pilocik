#ifndef PILIBOCIK_RESTRICTION_H
#define PILIBOCIK_RESTRICTION_H

#include <qglobal.h>

namespace PiLibocik{namespace Partition{
class Way;
class Node;

class Restriction{
private:
    qint64 to;
    qint64 via;
    int type;
public:
    Restriction();
    Restriction(qint64 to, qint64 via, int type);
    int getType();
    quint64 getWayTo();
    quint64 getVia();    
    Way getWayToObj();
    Node getViaObj();
};

}}


#endif // RESTRICTION_H
