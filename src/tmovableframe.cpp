#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include "tmovableframe.h"

TMovableFrame::TMovableFrame(QWidget *a):QFrame(a)
{
}


void TMovableFrame::setMode(TMovableFrame::TMOVABLEMODE mode){
    this->blockSignals(true);
    this->mode=mode;
}

void TMovableFrame::mousePressEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        if (isWindow())
            offset = event->globalPos() - pos();
        else
            offset = event->pos();
    }
}

void TMovableFrame::mouseMoveEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        if (isWindow())
            move(event->globalPos() - offset);
        else
           move(mapToParent(event->pos() - offset));
    }
}

void TMovableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        offset = QPoint();
    }
}
