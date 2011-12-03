#include <QtCore/QCoreApplication>
// Define plugin definitions
#include "main.h"
#include "compasswidget.h"

#include <QtPlugin>
#include <QDebug>


Compass::Compass(){
}
Compass::~Compass(){
    delete compassWidget;
}


void Compass::init(QMap<QString, QString>settings){
    compassWidget=new CompassWidget();
}

void Compass::run (){
}

QList<PluginWidget*> Compass::getWidgets(){
    QList<PluginWidget*> l;
    l.append(compassWidget);
    return l;
}

void Compass::positionUpdated(GPSdata data){
    qDebug()<<data.angle;
    compassWidget->setAngle(data.angle);
}

QMap<QString,QString> Compass::getSettings(){
    QMap<QString,QString> ret;
    return ret;
}




Q_EXPORT_PLUGIN2(libcompass, Compass)







