#include <QtCore/QCoreApplication>
// Define plugin definitions
#include "main.h"
#include "clockwidget.h"

#include <QtPlugin>
#include <QDebug>



SimpleClock::SimpleClock(){
}

SimpleClock::~SimpleClock(){
    qDebug()<<"CLEAR!!";
    delete clockWidget;
}


void SimpleClock::init(){
    clockWidget=new TClockWidget();
}


void SimpleClock::run (){
    qDebug()<<"TESTOWY START";
}



QList<PluginWidget*> SimpleClock::getWidgets(){
    QList<PluginWidget*> l;
    l.append(clockWidget);
    return l;
}



Q_EXPORT_PLUGIN2(libsimpleclock, SimpleClock)







