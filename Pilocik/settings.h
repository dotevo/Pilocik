#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QMap>
#include <QTranslator>
#include <QApplication>

class Settings
{
private:
    static Settings *instance;
    QDomDocument* doc;

    QApplication *app;

    QTranslator *translator;

    //coreSettings
    QString mapPath,
            mapStylePath,
            layoutStylePath,
            simulationPath;

    //profileSettings
    QDomElement profileSettingsXMLNode;
    QMap<QString, QMap<QString, QString> > widgetsSettings;
    QString language;
    double  lat,
            lon;
    int     zoom;

    QMap<QString, QString> languages;

    void addLanguages() {
        languages.insert("System", "System");
        //languages.insert("Systemowy", "system");
        //languages.insert("Polish", "pl_PL");
        languages.insert("Polski", "pl_PL");
        languages.insert("English", "en_EN");
        //languages.insert("Angielski", "en_EN");
        //languages.insert("system", "system");
        //languages.insert("systemowy", "system");
        //languages.insert("polish", "pl_PL");
        //languages.insert("polski", "pl_PL");
        //languages.insert("english", "en_EN");
        //languages.insert("angielski", "en_EN");
    }

public:
    Settings(QApplication* a = 0);
    ~Settings();
    static Settings* getInstance(QApplication* a = 0);
    void loadSettings();
    void saveSettings();

    void setLanguage(QString lang);
    QString getLanguage();
    QString getLanguageFromLocale(QString loc);
    QString getLocale(QString lang);

    void resetDefaultSettings();
    void configureProfile(QString profile);

    void modifyCoreSettings(QString name, QString value);
    void modifyWidgetSettings(QString name, QMap<QString,QString>);
    void modifyMapSettings(double lat, double lon, int zoom);
    void modifyLanguageSettings();

    QTranslator* reloadTranslation();

    QMap<QString,QString> getWidgetSettings(QString name);

    QString getMapPath()
    {
        return mapPath;
    }

    QString getMapStylePath()
    {
        return mapStylePath;
    }

    QString getLayoutStylePath()
    {
        return layoutStylePath;
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
