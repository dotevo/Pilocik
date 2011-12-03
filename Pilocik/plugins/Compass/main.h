#ifndef MAIN_H
#define MAIN_H

#include "../../pluginmanager.h"
#include "../../plugininterface.h"
#include "../../gpsdata.h"
#include <QDebug>

class CompassWidget;

class Compass :public QObject, public PluginInterface{
    Q_OBJECT
    Q_INTERFACES (PluginInterface)
    private:
        //Widgety
        CompassWidget *compassWidget;

    public:
        Compass();
        ~Compass();
        //From interface
        QString getName(){return "Compass";}
        QString getDesc(){return "TODO: descriotion";}
        PluginVersion getVersion(){static PluginVersion v(0,1); return v;}
        QList<PluginWidget*> getWidgets();
        void init (QMap<QString,QString>);
        void run ();
        QMap<QString,QString> getSettings();
public slots:
    void positionUpdated(GPSdata);

};


#endif // MAIN_H
