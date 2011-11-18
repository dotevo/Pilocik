#ifndef PILIBOCIK_RESTRICTION_H
#define PILIBOCIK_RESTRICTION_H

#include <pilibocik/partition/way.h>

namespace PiLibocik{namespace Partition{
class Way;

class Restriction{
private:
    quint64 to;
    quint64 via;
    int type;
public:
    Restriction();
    Restriction(quint64 to, quint64 via, int type);
    int getType();
    quint64 getWayTo();
    quint64 getVia();
    Way getWayToObj();
};

}}


#endif // RESTRICTION_H
