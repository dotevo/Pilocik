#ifndef RESTRICTION_H
#define RESTRICTION_H

#include <pilibocik/partition/way.h>

namespace PiLibocik{namespace Partition{
class Way;

class Restriction{
private:
    int type;
    quint64 to;
public:
    Restriction();
    int getType();
    quint64 getWayTo();
    Way getWayToObj();
};

}}


#endif // RESTRICTION_H
