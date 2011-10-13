#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include "tmovableframe.h"
#include <math.h>

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
            float xx=this->pos().x();
            float yy=this->pos().y();
            float x=event->pos().x()+xx;
            float y=event->pos().y()+yy;
            float alpha;

            if ((x > xx) && (y >= yy))                              // xx i yy to pozycje œrodka trójk¹ta
                alpha = ((float)atan((y - yy) / (x - xx)))*180/M_PI;
             else if ((x > xx) && (y < yy))
                alpha = (float)(atan((y-yy)/(x-xx)) + 2 * M_PI)*180/M_PI;
             else if(x < xx)
                alpha = (float)(atan((y-yy)/(x-xx)) + M_PI)*180/M_PI;
             else if ((x == xx) && (y > yy))
                alpha = (float)2*180;
             else
                alpha = (float)(3 *180 / 2);

            alpha=((int)alpha/45)*45;
            qDebug()<<xx<<":"<<yy<<";"<<x<<":"<<y<<"M\n"<<"Q"<<alpha<<"\n";
            proxy->rotate(alpha-proxy->rotation());
        }else{
            float x=event->pos().x();
            x/=this->width();
            float y=event->pos().y();
            y/=this->height();
            qDebug()<<x<<":"<<y<<";"<<event->pos().x()<<":"<<event->pos().y()<<"\n";
            if(x>0&&y>0&&proxy->size().width()>1&&proxy->size().height()>1)
            proxy->scale(x,y);
        }
        //offset=event->pos();

    }
}

void TMovableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if(mode==MOVING){
        event->accept(); // do not propagate
        offset = QPoint();
    }
}
