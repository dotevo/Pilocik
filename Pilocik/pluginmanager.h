#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include<QMap>
#include<QPluginLoader>

class Version{
private:
    int firstNumber;
    int secondNumber;
public:
    Version (int firstNumber, int secondNumber):firstNumber(firstNumber),secondNumber(secondNumber){}
    //TODO: opratory
};

class PluginInterface{
    public:
        virtual ~PluginInterface () {}
        virtual QString getName()=0;
        virtual QString getDesc()=0;
        virtual Version getVersion()=0;
        virtual void init ()=0;
        virtual void run ()=0;
};

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

Q_DECLARE_INTERFACE (PluginInterface, "PilocikPlugin/0.1")

#endif // PLUGINMANAGER_H
