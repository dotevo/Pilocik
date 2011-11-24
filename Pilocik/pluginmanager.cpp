#include "pluginmanager.h"


PluginManager* PluginManager::pluginManager = NULL;

PluginManager::PluginManager(){
    //�cie�ka do programu
    QDir plugins_dir(QApplication::applicationDirPath()+"/Plugins");
    //Filtr (pokazywanie plik�w *.so/dll)
    QStringList filters;

    #ifdef Q_OS_WIN32
        filters << "*.dll";
    #endif
    #ifdef Q_OS_LINUX
        filters << "*.so";
    #endif

    plugins_dir.setNameFilters(filters);
    //Lista plik�w w katalogu
    QStringList filelist=plugins_dir.entryList();
    QStringListIterator filelist_iterator(filelist);
    cout<<"�adowanie plugin�w:"<<endl;
    while (filelist_iterator.hasNext()){
        QString name=filelist_iterator.next().toLocal8Bit().constData();
        cout<<"\t"<<name.toStdString().c_str();
        QPluginLoader *loader=new QPluginLoader(QApplication::applicationDirPath()+"/Plugins/"+name);
        if(loader->load()){
            if (Plugin_Interface *interface = qobject_cast<Plugin_Interface *>(loader->instance())){
                //Sprawdzanie czy nie ma ju� pluginu danego typu np. baza danych
                plugins[interface->getName()]=loader;
            }else
                cout <<"\t\t [INIT ERROR]"<<endl;
        }else{
            cout <<"\t\t [LOAD ERROR]"<<endl;
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
        PluginInterface *pi=qobject_cast<PluginInterface *>(i.next().value()->instance());
        pi->run();
    }
}


PluginManager *PluginManager::getInstance(){
    if(pluginManager==NULL)
        pluginManager=new PluginManager();
    return pluginManager;
}

PluginInterface *PluginManager::getPlugin(QString name){
    if(plugins.find(name)==plugins.end()) return NULL;
    return qobject_cast<Plugin_Interface *>(plugins[name]->instance());
}

