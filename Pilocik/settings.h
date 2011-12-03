#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QMap>
#include <QTranslator>
#include <QApplication>
#include <pilibocik/poi.h>


class StorePoint
{
private:
    int     pos;
    QString name;
    double  lon,
            lat;

public:
    StorePoint(int pos, QString name, double lon, double lat);

    int getPos(){
        return pos;
    }

    QString getName(){
        return name;
    }

    double getLon(){
        return lon;
    }

    double getLat(){
        return lat;
    }
};

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
            poiFilePath,
            poiIconsDir,
            simulationPath;
    QMap<int, PiLibocik::PoiDisplay> poiDisplaySettings;


    //profileSettings
    QDomElement profileSettingsXMLNode;
    QMap<QString, QMap<QString, QString> > widgetsSettings;
    QMap<QString, QMap<QString, QString> > pluginsSettings;
    QList<StorePoint> historyPoints;
    QList<StorePoint> favouritePoints;
    QString language;
    QString startLanguage;
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
    QMap<QString,QString> getPluginSettings(QString pluginName);

    void resetDefaultSettings();
    void configureProfile(QString profile);

    void modifyCoreSettings(QString name, QString value);
    void modifyWidgetSettings(QString name, QMap<QString,QString>);
    void modifyMapSettings(double lat, double lon, int zoom);
    void modifyLanguageSettings();
    void modifyPoiDisplaySettings(QMap<int, PiLibocik::PoiDisplay> newPoiDisplaySettings);
    void modifyPluginSettings(QString pluginName,QMap<QString,QString> setting);

    void addHistoryPoint(QString name, double lon, double lat);
    void addFavouritePoint(QString name, double lon, double lat);
    void removeHistoryPoint(int pos);
    void removeFavouritePoint(int pos);

    QTranslator* reloadTranslation(QString lang = "");

    QMap<QString,QString> getWidgetSettings(QString name);

    QString getStartLanguage();

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

    QString getPoiFilePath()
    {
        return poiFilePath;
    }

    QString getPoiIconsDir()
    {
        return poiIconsDir;
    }

    QMap<int, PiLibocik::PoiDisplay> getPoiDisplaySettings()
    {
        return poiDisplaySettings;
    }

    QList<StorePoint> getHistoryPoints()
    {
        QList<StorePoint> revHistory;
        for(int i = historyPoints.size()-1; i>=0; i--)
            revHistory.append(historyPoints.at(i));
        return revHistory;
    }

    QList<StorePoint> getFavouritePoints()
    {
        return favouritePoints;
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
