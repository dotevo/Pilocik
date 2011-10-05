#include "twidgetmanager.h"
#include "twidget.h"

#include <QMapIterator>

TWidgetManager::TWidgetManager(){
}

TWidgetManager& TWidgetManager::getInstance(){
    static TWidgetManager instance;
    return instance;
}


TWidget* TWidgetManager::getWidget(QString name){
    return 0;
}

QList<TWidget*> TWidgetManager::getWidgetList(){
	QList<TWidget*> lista;
	return lista;
}

QList<TWidget*> TWidgetManager::getWidgetVisibleList(){
    QList<TWidget*> lista;
	return lista;
}

void TWidgetManager::setMode(TMovableFrame::TMOVABLEMODE mode){
    QMapIterator<QString, TWidget*> i(widgets);
     while (i.hasNext()) {
         i.next();
         i.value()->setMode(mode);
     }
}

void TWidgetManager::addWidget(QString name, TWidget* w){
    if(w!=0){
        widgets.insert(name,w);
        w->setParent(parent);
    }
}

void TWidgetManager::setParent(QWidget* w){
    //TODO: Zmiana widgetów
    parent=w;
}
