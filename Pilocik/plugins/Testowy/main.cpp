#include <QtCore/QCoreApplication>
// Define plugin definitions
#include "main.h"

#include <QtPlugin>
#include <QDebug>



Testowy::Testowy(){
}

void Testowy::init(){
    qDebug()<<"TESTOWY INIT";
}


void Testowy::run (){
    qDebug()<<"TESTOWY START";
}

QList<PluginWidget*> Testowy::getWidgets(){
    QList<PluginWidget*> l;
    return l;
}


Q_EXPORT_PLUGIN2(libtestowy, Testowy)







