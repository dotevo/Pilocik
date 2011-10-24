#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QMap>

class Settings
{
private:
    static Settings *instance;
    QDomDocument* doc;

    //coreSettings
    QString mapPath,
            mapStylePath,
            simulationPath;

    //profileSettings
    QDomElement profileSettingsXMLNode;
    QMap<QString, QMap<QString, QString> > widgetsSettings;
    double  lat,
            lon;
    int     zoom;

public:
    Settings();
    ~Settings();
    static Settings* getInstance();
    void loadSettings();
    void saveSettings();

    void resetDefaultSettings();
    void configureProfile(QString profile);

    void modifyCoreSettings(QString name, QString value);
    void modifyWidgetSettings(QString name, QMap<QString,QString>);
    void modifyMapSettings(double lat, double lon, int zoom);

    QMap<QString,QString> getWidgetSettings(QString name);

    QString getMapPath()
    {
        return mapPath;
    }

    QString getMapStylePath()
    {
        return mapStylePath;
    }

    QString getSimulationPath()
    {
        return simulationPath;
    }

    double getLat()
    {
        return lat;
    }

    double getLon()
    {
        return lon;
    }

    int getZoom()
    {
        return zoom;
    }

};

#endif // SETTINGS_H
