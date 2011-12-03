#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QMap>

class PluginWidget;
class GPSData;

/**
 * @brief
 *
 * @class PluginVersion plugininterface.h "plugininterface.h"
 */
class PluginVersion{
private:
    int firstNumber; /**< First number of version*/
    int secondNumber; /**< Second number of version */
public:
/**
 * @brief
 *
 * @fn PluginVersion
 * @param firstNumber
 * @param secondNumber
 */
    PluginVersion (int firstNumber, int secondNumber):firstNumber(firstNumber),secondNumber(secondNumber){}
};

/**
 * @brief
 *
 * @class PluginInterface plugininterface.h "plugininterface.h"
 */
class PluginInterface{
public:
    /**
     * @brief
     *
     * @fn ~PluginInterface
     */
    virtual ~PluginInterface () {}
    /**
     * @brief get plugins's name
     *
     * @fn getName
     */
    virtual QString getName()=0;
    /**
     * @brief get plugin's description
     *
     * @fn getDesc
     */
    virtual QString getDesc()=0;
    /**
     * @brief get plugin's version
     *
     * @fn getVersion
     */
    virtual PluginVersion getVersion()=0;
    /**
     * @brief get plugin's widgets (on main screen)
     *
     * @fn getWidgets
     */
    virtual QList<PluginWidget*> getWidgets()=0;
    /**
     * @brief init plugin in this phase, plugin makes objects
     *
     * @fn init
     */
    virtual void init (QMap<QString,QString> settins)=0;
    /**
     * @brief run all needed functions (eg. timers)
     *
     * @fn run
     */
    virtual void run ()=0;
    /**
     * @brief get settings of plugin. Using before unload.
     *
     * @fn
     */
    virtual QMap<QString,QString> getSettings()=0;
};

Q_DECLARE_INTERFACE (PluginInterface, "PilocikPlugin/0.1")


#endif // PLUGININTERFACE_H
