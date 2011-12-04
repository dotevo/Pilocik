#include "pluginmanager.h"
#include "pluginwidget.h"
#include "plugininterface.h"
#include "twidgetmanager.h"
#include "navigationwindow.h"
#include "gpsreceiver.h"
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

    qDebug()<<"Dodawanie pluginów:";
    while (filelist_iterator.hasNext()){
        QString name=filelist_iterator.next().toLocal8Bit().constData();
        qDebug()<<"\t"<<name;
        QPluginLoader *loader=new QPluginLoader(QApplication::applicationDirPath()+"/Plugins/"+name);
        pluginsList.append(loader);
    }
}

void PluginManager::loadPlugins(){
    //todo Load plugins from settings
    initAll();
    runAll();
}

QPluginLoader* PluginManager::getPlugin(QString name){
    QListIterator<QPluginLoader*> iter(pluginsList);
    while(iter.hasNext()){
        QPluginLoader* a=iter.next();
        QFileInfo pathInfo( a->fileName() );
        if(pathInfo.fileName()==name)
            return a;
    }
    return 0;
}

void PluginManager::loadPlugin(QString name){
    QPluginLoader *a=getPlugin(name);
    qDebug()<<"LoadPlugin:"<<name<<":"<<(a==0);
    if(a==0)return;
    if(!a->load()) return;
    PluginInterface *pi=qobject_cast<PluginInterface *>(a->instance());
    if(pi==0)return;

    QMap<QString,QString> settings=Settings::getInstance()->getPluginSettings(pi->getName());
    pi->init(settings);
    pi->run();

    QObject *piObj=qobject_cast<QObject *>(a->instance());
    if(piObj!=0){
        //Connect slots&signal
       // if  (piObj->metaObject()->indexOfSlot( "positionUpdated" ) != -1){
            QObject::connect(NavigationWindow::main->gps, SIGNAL(positionUpdate(GPSdata)), piObj, SLOT(positionUpdated(GPSdata)));
        //}else{
         //   qDebug()<<"Brak slotu positionUpdated";
        //}
    }



    QListIterator <PluginWidget*> plugin(pi->getWidgets());
    while(plugin.hasNext()){
        PluginWidget *n=plugin.next();
        n->setParent(NavigationWindow::main);
        n->setVisible(false);
        TWidgetManager::getInstance()->addWidget(n->getWidgetName(), n);
        qDebug()<<"ADD widget"<<n->getWidgetName();
    }

}

void PluginManager::unloadPlugin(QString name){
    QPluginLoader *a=getPlugin(name);
    if(a==0)return;

    PluginInterface *pi=qobject_cast<PluginInterface *>(a->instance());
    Settings::getInstance()->modifyPluginSettings(pi->getName(),pi->getSettings());
    QListIterator <PluginWidget*> plugin(pi->getWidgets());
    while(plugin.hasNext()){
        PluginWidget *n=plugin.next();
        TWidgetManager::getInstance()->removeWidget(n->getWidgetName());
    }


    if(!a->unload())return;

}

void PluginManager::initAll(){
    QListIterator<QPluginLoader*> i(pluginsList);
    while (i.hasNext()) {
        QPluginLoader *a=i.next();
        if(a->isLoaded()){
            PluginInterface *pi=qobject_cast<PluginInterface *>(a->instance());


            QMap<QString,QString> settings=Settings::getInstance()->getPluginSettings(pi->getName());
            pi->init(settings);
        }
    }
}

void PluginManager::runAll(){
    QListIterator<QPluginLoader*> i(pluginsList);
    while (i.hasNext()) {
        QPluginLoader *a=i.next();
        if(a->isLoaded()){
            PluginInterface *pi=qobject_cast<PluginInterface *>(a->instance());
            pi->run();
            QListIterator <PluginWidget*> plugin(pi->getWidgets());
            while(plugin.hasNext()){
                PluginWidget *n=plugin.next();
                n->setParent(NavigationWindow::main);
                TWidgetManager::getInstance()->addWidget(n->getWidgetName(), n);
                qDebug()<<"Dodane:"<<n->getWidgetName();
            }
        }
    }
}


PluginManager *PluginManager::getInstance(){
    if(pluginManager==NULL)
        pluginManager=new PluginManager();
    return pluginManager;
}


QList<QPluginLoader*>  PluginManager::getAllPlugins(){
    return pluginsList;
}

