#include <QEvent>
#include <QDebug>
#include "tmovableframe.h"

TMovableFrame::TMovableFrame(QWidget *a):QFrame(a)
{
}

bool TMovableFrame::event ( QEvent * e ){
    qDebug()<<e->type();
    return false;
}
