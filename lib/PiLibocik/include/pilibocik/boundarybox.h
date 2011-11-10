#ifndef BOUNDARYBOX_H
#define BOUNDARYBOX_H

#include <pilibocik/point.h>

namespace PiLibocik{


class BoundaryBox{
private:
    Point p1;
    Point p2;
public:
    BoundaryBox(Point p1,Point p2);
};

}

#endif // BOUNDARYBOX_H
