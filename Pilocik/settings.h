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

    void setPos(int pos){
        this->pos = pos;
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

    /**
      @brief Returns instance of settings singleton.
      */
    static Settings* getInstance(QApplication* a = 0);
    /**
      @brief Loads settings from file to variables.
      */
    void loadSettings();
    /**
      @brief Saves settings from memory to file.
      */
    void saveSettings();

    /**
      @brief Sets language setting.
      @param lang Language
      */
    void setLanguage(QString lang);

    /**
      @brief Gets actual language setting.
      @return Language.
      */
    QString getLanguage();
    QString getLanguageFromLocale(QString loc);
    QString getLocale(QString lang);

    /**
      @brief Return settgins of specified plugin.
      @param pluginName Plugin name.
      */
    QMap<QString,QString> getPluginSettings(QString pluginName);

    /**
      @brief Creates new settings file if it's not existing.
      */
    void resetDefaultSettings();

    /**
      @brief Loads settings assigned to specified profile.
      @param profile Profile name.
      */
    void configureProfile(QString profile);

    /**
      @brief Modifies main settings.
      @param name Setting name.
      @param value Setting value.
      */
    void modifyCoreSettings(QString name, QString value);

    /**
      @brief Modifies settings of specified widget.
      @param name Widget name.
      @param value Settings as QMap.
      */
    void modifyWidgetSettings(QString name, QMap<QString,QString> settingsToSave);

    /**
      @brief Modifies map settings.
      @param lat New longitude value.
      @param lon New latitude value.
      @param zoom New zoom value.
      */
    void modifyMapSettings(double lat, double lon, int zoom);

    /**
      @brief Modifies language settings.
      */
    void modifyLanguageSettings();

    /**
      @brief Modifies poi displaying settings.
      @param newPoiDisplaySettings QMap containing all poi display settings.
      */
    void modifyPoiDisplaySettings(QMap<int, PiLibocik::PoiDisplay> newPoiDisplaySettings);

    /**
      @brief Modifies plugin settings.
      @param pluginName Plugin name.
      @param setting QMap containing all current plugin settings.
      */
    void modifyPluginSettings(QString pluginName,QMap<QString,QString> setting);

    /**
      @brief Adds new favourite point.
      @param name Favourite point name.
      @param lon Longitude.
      @param lat Latitude.
      */
    void addHistoryPoint(QString name, double lon, double lat);

    /**
      @brief Adds new history point.
      @param name History point name.
      @param lon Longitude.
      @param lat Latitude.
      */
    void addFavouritePoint(QString name, double lon, double lat);

    /**
      @brief Deletes history point.
      @param pos History point position.
      */
    void removeHistoryPoint(int pos);

    /**
      @brief Deletes favourite point.
      @param pos Favourite point position.
      */
    void removeFavouritePoint(int pos);

    /**
      @brief Reloads translation.
      @param lang Language
      */
    QTranslator* reloadTranslation(QString lang = "");

    /**
      @brief Returns wiget settings.
      @param name Widget name
      @return QMap containing all settings of specified widget.
      */
    QMap<QString,QString> getWidgetSettings(QString name);

    /**
      @brief Returns startup language.
      @return Language.
      */
    QString getStartLanguage();

    /**
      @brief Returns map path.
      */
    QString getMapPath()
    {
        return mapPath;
    }

    /**
      @brief Returns map style path.
      */
    QString getMapStylePath()
    {
        return mapStylePath;
    }

    /**
      @brief Returns layout style path.
      */
    QString getLayoutStylePath()
    {
        return layoutStylePath;
    }

    /**
      @brief Returns simulation path.
      */
    QString getSimulationPath()
    {
        return simulationPath;
    }

    /**
      @brief Returns poi file path.
      */
    QString getPoiFilePath()
    {
        return poiFilePath;
    }

    /**
      @brief Returns poi icons location.
      */
    QString getPoiIconsDir()
    {
        return poiIconsDir;
    }

    /**
      @brief Returns poi display settings.
      */
    QMap<int, PiLibocik::PoiDisplay> getPoiDisplaySettings()
    {
        return poiDisplaySettings;
    }

    /**
      @brief Returns all stored history points in reverse order.
      */
    QList<StorePoint> getHistoryPoints()
    {
        QList<StorePoint> revHistory;
        for(int i = historyPoints.size()-1; i>=0; i--)
            revHistory.append(historyPoints.at(i));
        return revHistory;
    }

    /**
      @brief Returns all stored favourite points.
      */
    QList<StorePoint> getFavouritePoints()
    {
        return favouritePoints;
    }

    /**
      @brief Returns stored map latitude.
      */
    double getLat()
    {
        return lat;
    }

    /**
      @brief Returns stored map longitude.
      */
    double getLon()
    {
        return lon;
    }

    /**
      @brief Returns stored map zoom.
      */
    int getZoom()
    {
        return zoom;
    }

};

#endif // SETTINGS_H
