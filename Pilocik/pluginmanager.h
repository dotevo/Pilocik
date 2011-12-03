#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include<QMap>
#include<QList>
#include<QPluginLoader>

class PluginWidget;
class PluginInterface;

class PluginManager
{
private:
    PluginManager();
    QList<QPluginLoader*> pluginsList;
    QMap<QString,QPluginLoader*> plugins;
    static PluginManager *pluginManager;
    void initAll();
    void runAll();
public:
    static PluginManager * getInstance();
    void loadPlugins();
    void loadPlugin(QString name);
    void unloadPlugin(QString name);

    QPluginLoader*         getPlugin(QString name);
    QList<QPluginLoader*>  getAllPlugins();
    PluginInterface       *getPluginFromType(QString type);
};


#endif // PLUGINMANAGER_H
