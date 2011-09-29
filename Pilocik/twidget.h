#ifndef TWIDGET_H
#define TWIDGET_H

#include <QString>
#include "tmovableframe.h"
class QWidget;

class TWidget: public TMovableFrame
{
public:
    TWidget(QWidget *parent=0);
};

#endif // TWIDGET_H
