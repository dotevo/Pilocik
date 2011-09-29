#ifndef TWIDGETMANAGER_H
#define TWIDGETMANAGER_H

#include <QMap>

#include "tmovableframe.h"

class TWidget;

//singleton
class TWidgetManager
{
private:
    TWidgetManager();
    QMap <QString, TWidget*> widgets;
    QWidget *parent;
public:
    static TWidgetManager& getInstance();
    TWidgetManager(TWidgetManager const&);              // Don't Implement
    void operator=(TWidgetManager const&);          // Don't implement

    void setWidgetVisible(QString name,bool visible);
    TWidget* getWidget(QString name);
    QList<TWidget*> getWidgetList();
    QList<TWidget*> getWidgetVisibleList();
    void addWidget(QString name, TWidget* w);
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    void setParent(QWidget *w);
};

#endif // TWIDGETMANAGER_H
