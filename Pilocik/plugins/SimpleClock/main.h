#ifndef MAIN_H
#define MAIN_H

#include "../../pluginmanager.h"
#include "../../plugininterface.h"
#include <QDebug>

class TClockWidget;

class SimpleClock :public QObject, public PluginInterface{
    Q_OBJECT
    Q_INTERFACES (PluginInterface)
    private:
        PluginManager *plugin_manager;
        //Widgety
        TClockWidget *clockWidget;

    public:
        SimpleClock();
        //From interface
        QString getName(){return "SimpleClock";}
        QString getDesc(){return "SimpleClock";}
        PluginVersion getVersion(){static PluginVersion v(0,1); return v;}
        QList<PluginWidget*> getWidgets();
        void init ();
        void run ();
    };


#endif // MAIN_H
