#ifndef TWIDGETMANAGER_H
#define TWIDGETMANAGER_H

#include <QMap>

#include "tmovableframe.h"

class TWidget;

//singleton
class TWidgetManager: public QGraphicsScene
{
private:
    TWidgetManager();
    QMap <QString, TMovableFrame*> widgets;
    QWidget *parent;
    static TWidgetManager *instance;
public:
    ~TWidgetManager();

    typedef QGraphicsScene qGraphicsScene;

    static TWidgetManager* getInstance();
    TWidgetManager(TWidgetManager const&);              // Don't Implement
    void operator=(TWidgetManager const&);          // Don't implement

    void setWidgetVisible(QString name,bool visible);
    TMovableFrame* getWidget(QString name);
    QList<TMovableFrame*> getWidgetList();
    QList<TMovableFrame*> getWidgetVisibleList();
    void addWidget(QString name, TMovableFrame* w);
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    void setParent(QWidget *w);
};

#endif // TWIDGETMANAGER_H
