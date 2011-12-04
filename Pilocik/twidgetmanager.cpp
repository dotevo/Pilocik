#include "twidgetmanager.h"
#include "settings.h"

#include <QMapIterator>
#include <iostream>

TWidgetManager * TWidgetManager::instance=0;

TWidgetManager::TWidgetManager() {
    ROUTING = false;
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

void TWidgetManager::setWidgetVisible(QString name, bool visible)
{
    widgets[name]->setVisible(visible);
}

TMovableFrame::TMOVABLEMODE TWidgetManager::getMode() {
    QMapIterator<QString, TMovableFrame*> i(widgets);
    i.next();
    return i.value()->getMode();
}

void TWidgetManager::addWidget(QString name, TMovableFrame* w) {
    if(w!=0){
        widgets.insert(name,w);

        //Configure widget from settings file
        QMap<QString,QString> widgetSettings = Settings::getInstance()->getWidgetSettings(name);
        int PosX=0;
        int PosY=0;
        if(widgetSettings.contains("posx"))
            PosX=widgetSettings["posx"].toInt();
        if(widgetSettings.contains("posy"))
            PosY=widgetSettings["posy"].toInt();

        QPoint position(PosX,PosY);
        w->move(position);
        //w->setVisible(widgetSettings["enabled"]=="true");

        w->setParent(parent);
    }
}

void TWidgetManager::removeWidget(QString name) {
    widgets.remove(name);
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
        if (!ROUTING && i.key().compare("Hint") == 0) {
            i.value()->setVisible(false);
        }
        else {
            i.value()->setVisible(true);
         //   i.value()->update();
        }
    }
}

void TWidgetManager::setRouting(bool mode)
{
    ROUTING = &mode;
}
