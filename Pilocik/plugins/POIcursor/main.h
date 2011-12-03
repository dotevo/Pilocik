#ifndef MAIN_H
#define MAIN_H

#include "../../pluginmanager.h"
#include "../../plugininterface.h"
#include "../../gpsdata.h"
#include <QDebug>


class POIcursor :public QObject, public PluginInterface{
    Q_OBJECT
    Q_INTERFACES (PluginInterface)
    private:
    public:
        POIcursor();
        ~POIcursor();
        //From interface
        QString getName(){return "POIcursor";}
        QString getDesc(){return "Show you, where is the nearest POI";}
        PluginVersion getVersion(){static PluginVersion v(0,1); return v;}
        QList<PluginWidget*> getWidgets();
        void init ();
        void run (QMap<QString,QString> settings);
        QMap<QString,QString> getSettings();
public slots:
    void positionUpdated(GPSdata);

};


#endif // MAIN_H
