#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include "tmovableframe.h"
#include <math.h>

TMovableFrame::TMovableFrame(QWidget *parent):QFrame(parent){
}


void TMovableFrame::setMode(TMovableFrame::TMOVABLEMODE mode){
    this->blockSignals(true);
    this->mode=mode;
}

void TMovableFrame::mousePressEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        offset = event->pos();
    }
}

void TMovableFrame::mouseMoveEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
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
