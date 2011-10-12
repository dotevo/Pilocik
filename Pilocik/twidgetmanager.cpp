#include "twidgetmanager.h"

#include <QMapIterator>

TWidgetManager::TWidgetManager():qGraphicsScene(0,0,1,1){
}

TWidgetManager& TWidgetManager::getInstance(){
    static TWidgetManager instance;
    return instance;
}


TMovableFrame* TWidgetManager::getWidget(QString name){
    return 0;
}

QList<TMovableFrame*> TWidgetManager::getWidgetList(){
        QList<TMovableFrame*> lista;
	return lista;
}

QList<TMovableFrame*> TWidgetManager::getWidgetVisibleList(){
    QList<TMovableFrame*> lista;
	return lista;
}

void TWidgetManager::setMode(TMovableFrame::TMOVABLEMODE mode){
    QMapIterator<QString, TMovableFrame*> i(widgets);
     while (i.hasNext()) {
         i.next();
         i.value()->setMode(mode);
     }
}

void TWidgetManager::addWidget(QString name, TMovableFrame* w){
    if(w!=0){
        widgets.insert(name,w);
        w->setParent(parent);
    }
}

void TWidgetManager::setParent(QWidget* w){
    //TODO: Zmiana widgetów
    parent=w;
}
