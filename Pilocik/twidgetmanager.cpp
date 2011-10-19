#include "twidgetmanager.h"

#include <QMapIterator>

TWidgetManager * TWidgetManager::instance=0;

TWidgetManager::TWidgetManager(){
}

TWidgetManager::~TWidgetManager(){
}

TWidgetManager* TWidgetManager::getInstance(){
    if(instance==0)
        instance=new TWidgetManager();
    return instance;
}


TMovableFrame* TWidgetManager::getWidget(QString name){
    return widgets[name];
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
