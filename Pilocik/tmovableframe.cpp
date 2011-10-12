#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include "tmovableframe.h"

TMovableFrame::TMovableFrame(QGraphicsScene *a):QFrame()
{
    proxy=a->addWidget(this);
    proxy->setFlag(QGraphicsItem::ItemIsSelectable);
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
        //Rotate
        if(offset.x()<this->width()/4)
            type=1;
        //Size
        else if(offset.x()>(this->width()*3)/4)
            type=2;
        //move
        else
            type=0;
    }
}

void TMovableFrame::mouseMoveEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        if(type==0)
            move(mapToParent(event->pos() - offset));
        else if(type==1){
            qreal ang=(event->pos().x() - offset.x())/proxy->scene()->width();
            proxy->rotate(proxy->rotation()+ang);
        }else{
            QPointF x=proxy->mapFromScene(event->pos().x() - offset.x(),event->pos().y() - offset.y());

            qDebug()<<x.x()<<":"<<x.y()<<"\n";
            proxy->scale(x.x(),x.y());
        }

    }
}

void TMovableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        offset = QPoint();
    }
}
