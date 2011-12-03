#include <QtCore/QCoreApplication>
// Define plugin definitions
#include "main.h"

#include <QtPlugin>
#include <QDebug>


POIcursor::POIcursor(){
}
POIcursor::~POIcursor(){
    delete compassWidget;
}


void POIcursor::init(){
    compassWidget=new CompassWidget();
}

void POIcursor::run (QMap<QString,QString> settings){
}

QList<PluginWidget*> POIcursor::getWidgets(){
    QList<PluginWidget*> l;
    l.append(compassWidget);
    return l;
}

void POIcursor::positionUpdated(GPSdata data){
}



Q_EXPORT_PLUGIN2(libpoicursor, POIcursor)







