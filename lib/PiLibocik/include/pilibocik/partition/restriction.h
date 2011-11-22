#ifndef RESTRICTION_H
#define RESTRICTION_H

#include <pilibocik/partition/way.h>

namespace PiLibocik{namespace Partition{
class Way;

class Restriction{
private:
    int type;
    quint64 via;
    quint64 to;
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
