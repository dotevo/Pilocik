#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QFrame>
#include "tmovableframe.h"

class PluginWidget: public TMovableFrame{
public:
    explicit PluginWidget(QWidget *parent = 0):TMovableFrame(parent){}
    virtual QString getWidgetName()=0;
};


#endif // PLUGINWIDGET_H
