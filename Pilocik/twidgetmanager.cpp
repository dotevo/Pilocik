#include "twidgetmanager.h"
#include "settings.h"

#include <QMapIterator>
#include <iostream>

TWidgetManager * TWidgetManager::instance=0;

TWidgetManager::TWidgetManager() {
}

TWidgetManager::~TWidgetManager() {
    QMapIterator<QString, TMovableFrame*> i(widgets);
    while (i.hasNext()) {
        i.next();
        QMap<QString, QString> settings;
        settings.insert("enabled", i.value()->isVisible()?"false":"true");
        settings.insert("posx",QString::number(i.value()->pos().x()));
        settings.insert("posy",QString::number(i.value()->pos().y()));
        Settings::getInstance()->modifyWidgetSettings(i.key(), settings);
    }

}

TWidgetManager* TWidgetManager::getInstance() {
    if(instance==0)
        instance=new TWidgetManager();
    return instance;
}

TMovableFrame* TWidgetManager::getWidget(QString name) {
    return widgets[name];
}

QList<TMovableFrame*> TWidgetManager::getWidgetList() {
    QList<TMovableFrame*> lista;
    return lista;
}

QList<TMovableFrame*> TWidgetManager::getWidgetVisibleList() {
    QList<TMovableFrame*> lista;
    return lista;
}

void TWidgetManager::changeMode() {
    QMapIterator<QString, TMovableFrame*> i(widgets);
    if (i.hasNext()) {
        i.next();
        (i.value()->getMode() == TMovableFrame::STAND) ? setMode(TMovableFrame::MOVING) : setMode(TMovableFrame::STAND);
    }
}

void TWidgetManager::setMode(TMovableFrame::TMOVABLEMODE mode) {
    QMapIterator<QString, TMovableFrame*> i(widgets);
     while (i.hasNext()) {
         i.next();
         i.value()->setMode(mode);
     }
}

void TWidgetManager::addWidget(QString name, TMovableFrame* w) {
    if(w!=0){
        widgets.insert(name,w);

        //Configure widget from settings file
        QMap<QString,QString> widgetSettings = Settings::getInstance()->getWidgetSettings(name);
        QPoint position(widgetSettings["posx"].toInt(),widgetSettings["posy"].toInt());
        w->move(position);
        w->setVisible(widgetSettings["enabled"]=="true");

        w->setParent(parent);
    }
}

void TWidgetManager::setParent(QWidget* w) {
    //TODO: Zmiana widgetów
    parent=w;
}

void TWidgetManager::hideAllWidgets() {
    QMapIterator<QString, TMovableFrame*> i(widgets);
    while (i.hasNext()) {
        i.next();
        i.value()->setVisible(false);
    }
}

void TWidgetManager::showAllWidgets() {
    QMapIterator<QString, TMovableFrame*> i(widgets);
    while (i.hasNext()) {
        i.next();
        i.value()->setVisible(true);
    }
}
