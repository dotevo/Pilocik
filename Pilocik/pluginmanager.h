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
    QMap<QString,QPluginLoader*> plugins;
    static PluginManager *pluginManager;
public:
    static PluginManager * getInstance();
    void initAll();
    void runAll();
    PluginInterface *getPlugin(QString name);
    PluginInterface *getPluginFromType(QString type);
};


#endif // PLUGINMANAGER_H
