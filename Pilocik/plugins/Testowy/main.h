#ifndef MAIN_H
#define MAIN_H

#include "../../pluginmanager.h"
#include "../../plugininterface.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QAbstractTableModel>


class Testowy :public QObject, public PluginInterface{
    Q_OBJECT
    Q_INTERFACES (PluginInterface)
    private:
        PluginManager *plugin_manager;
    public:
        Testowy();
        //From interface
        QString getName(){return "Testowy";}
        QString getDesc(){return "Test";}
        PluginVersion getVersion(){static PluginVersion v(0,1); return v;}
        QList<PluginWidget*> getWidgets();

        void init ();
        void run ();
    };


#endif // MAIN_H

