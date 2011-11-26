#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>

class PluginWidget;

class PluginVersion{
private:
    int firstNumber;
    int secondNumber;
public:
    PluginVersion (int firstNumber, int secondNumber):firstNumber(firstNumber),secondNumber(secondNumber){}
};

class PluginInterface{
public:
    virtual ~PluginInterface () {}
    virtual QString getName()=0;
    virtual QString getDesc()=0;
    virtual PluginVersion getVersion()=0;
    virtual QList<PluginWidget*> getWidgets()=0;
    virtual void init ()=0;
    virtual void run ()=0;
};

Q_DECLARE_INTERFACE (PluginInterface, "PilocikPlugin/0.1")


#endif // PLUGININTERFACE_H
