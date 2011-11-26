#include "pluginmanager.h"
#include "pluginwidget.h"
#include "plugininterface.h"
#include "twidgetmanager.h"
#include "navigationwindow.h"
#include <QDebug>
#include <QDir>
#include <QHBoxLayout>
#include <QStringList>
#include <QApplication>
#include <QListIterator>

PluginManager* PluginManager::pluginManager = NULL;

PluginManager::PluginManager(){
    //Œcie¿ka do programu
    QDir plugins_dir(QApplication::applicationDirPath()+"/Plugins");
    //Filtr (pokazywanie plików *.so/dll)
    QStringList filters;

    #ifdef Q_OS_WIN32
        filters << "*.dll";
    #endif
    #ifdef Q_OS_LINUX
        filters << "*.so";
    #endif

    plugins_dir.setNameFilters(filters);
    //Lista plików w katalogu
    QStringList filelist=plugins_dir.entryList();
    QStringListIterator filelist_iterator(filelist);

    qDebug()<<"£adowanie pluginów:";
    while (filelist_iterator.hasNext()){
        QString name=filelist_iterator.next().toLocal8Bit().constData();
        qDebug()<<"\t"<<name;
        QPluginLoader *loader=new QPluginLoader(QApplication::applicationDirPath()+"/Plugins/"+name);
        if(loader->load()){
           PluginInterface *inter=(PluginInterface *)(loader->instance());

           if(inter !=0 ){
               QString n=inter->getName();
               if(!plugins.contains(n)){
                    //Sprawdzanie czy nie ma ju¿ pluginu danego typu np. baza danych
                    plugins.insert(n,loader);
                    qDebug() <<"\t\t [OK]";
               }
            }else
                qDebug() <<"\t\t [INIT ERROR]";
        }else{
            qDebug() <<"\t\t [LOAD ERROR]";
            delete loader;
        }
    }
}

void PluginManager::initAll(){
    QMapIterator<QString, QPluginLoader*> i(plugins);
    while (i.hasNext()) {
        PluginInterface *pi=qobject_cast<PluginInterface *>(i.next().value()->instance());
        pi->init();
    }
}

void PluginManager::runAll(){
    QMapIterator<QString, QPluginLoader*> i(plugins);
    while (i.hasNext()) {
        qDebug()<<plugins.size();
        PluginInterface *pi=qobject_cast<PluginInterface *>(i.next().value()->instance());
        pi->run();
        qDebug()<<pi->getName();
        //Add widgets
        QListIterator <PluginWidget*> plugin(pi->getWidgets());
        while(plugin.hasNext()){
            PluginWidget *n=plugin.next();
            n->setParent(NavigationWindow::main);
            TWidgetManager::getInstance()->addWidget(n->getWidgetName(), n);
            qDebug()<<"Dodane:"<<n->getWidgetName();
        }
    }
}


PluginManager *PluginManager::getInstance(){
    if(pluginManager==NULL)
        pluginManager=new PluginManager();
    return pluginManager;
}

PluginInterface *PluginManager::getPlugin(QString name){
    if(plugins.find(name)==plugins.end()) return NULL;
    return qobject_cast<PluginInterface *>(plugins[name]->instance());
}

