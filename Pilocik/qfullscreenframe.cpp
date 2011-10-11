#include "qfullscreenframe.h"
#include "navigationwindow.h"

QFullScreenFrame::QFullScreenFrame(NavigationWindow *parent) :
    QFrame(parent){
    connect(parent, SIGNAL(sizeChanged(QWidget*)), this, SLOT(sizeChanged(QWidget*)));    
}

void QFullScreenFrame::sizeChanged(QWidget *w){
    QRect rect2;
    rect2.setSize(w->size());
    rect2.setX(0);
    rect2.setY(0);
    setGeometry(rect2);
}
