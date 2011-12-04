#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include "settings.h"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QTranslator>

Settings * Settings::instance=0;

StorePoint::StorePoint(int pos, QString name, double lon, double lat)
{
    this->pos = pos;
    this->name = name;
    this->lon = lon;
    this->lat = lat;
}

Settings::Settings(QApplication* a)
{
    app = a;

    addLanguages();

    startLanguage = "";
}

Settings::~Settings(){
}

void Settings::loadSettings()
{
    doc = new QDomDocument();
    QFile file("settings.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        resetDefaultSettings();
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
    }
    doc->setContent(&file);
    file.close();
}

void Settings::saveSettings()
{
    modifyLanguageSettings();

    QFile file("settings.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    file.resize(0);
    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("UTF-8");
    doc->save(stream,4);
    file.close();
}

Settings* Settings::getInstance(QApplication* a)
{
    if(instance==0)
        instance=new Settings(a);
    return instance;
}

void Settings::resetDefaultSettings()
{
    QString defaultXML =
            "<?xml version='1.0' encoding='UTF-8'?> \n"
            "<settings>\n"
            "   <coreSettings>\n"
            "       <mapPath></mapPath>\n"
            "       <mapStylePath></mapStylePath>\n"
            "       <layoutStylePath></layoutStylePath>\n"
            "       <poiFilePath>c:/map/poi</poiFilePath>\n"
            "       <poiIconsDir>../Pilocik/images/poi/</poiIconsDir>\n"
            "       <simulationPath></simulationPath>\n"
            "   </coreSettings>\n"
            "   <poiSettings>\n"
            "       <poi zoom=\"128\" icon=\"transport_fuel.gif\" type=\"0\"/>\n"
            "       <poi zoom=\"8192\" icon=\"transport_parking.gif\" type=\"1\"/>\n"
            "       <poi zoom=\"16384\" icon=\"shopping_car.gif\" type=\"2\"/>\n"
            "       <poi zoom=\"16384\" icon=\"money_atm.gif\" type=\"3\"/>\n"
            "       <poi zoom=\"16384\" icon=\"money_bank.gif\" type=\"4\"/>\n"
            "       <poi zoom=\"8192\" icon=\"health_pharmacy.gif\" type=\"5\"/>\n"
            "       <poi zoom=\"8192\" icon=\"health_hospital.gif\" type=\"6\"/>\n"
            "       <poi zoom=\"8192\" icon=\"tourist_cinema2.gif\" type=\"7\"/>\n"
            "       <poi zoom=\"16384\" icon=\"nightclub.gif\" type=\"8\"/>\n"
            "       <poi zoom=\"8192\" icon=\"tourist_theatre.gif\" type=\"9\"/>\n"
            "       <poi zoom=\"8192\" icon=\"poi_embassy.gif\" type=\"10\"/>\n"
            "       <poi zoom=\"8192\" icon=\"amenity_firestation.gif\" type=\"11\"/>\n"
            "       <poi zoom=\"8192\" icon=\"amenity_police.n.gif\" type=\"12\"/>\n"
            "       <poi zoom=\"8192\" icon=\"amenity_post_office.gif\" type=\"13\"/>\n"
            "       <poi zoom=\"16384\" icon=\"accommodation_shelter.gif\" type=\"14\"/>\n"
            "       <poi zoom=\"16384\" icon=\"toilets.gif\" type=\"15\"/>\n"
            "       <poi zoom=\"16384\" icon=\"amenity_town_hall.gif\" type=\"16\"/>\n"
            "       <poi zoom=\"16384\" icon=\"money_currency_exchange.gif\" type=\"17\"/>\n"
            "       <poi zoom=\"16384\" icon=\"shopping_convenience.gif\" type=\"18\"/>\n"
            "       <poi zoom=\"16384\" icon=\"food_restaurant.gif\" type=\"19\"/>\n"
            "       <poi zoom=\"16384\" icon=\"food_bar.gif\" type=\"20\"/>\n"
            "       <poi zoom=\"16384\" icon=\"food_cafe.gif\" type=\"21\"/>\n"
            "   </poiSettings>\n"
            "   <profile name=\"default\">\n"
            "       <widgets>\n"
            "           <widget name=\"Clock\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>5</posx>\n"
            "               <posy>5</posy>\n"
            "           </widget>\n"
            "           <widget name=\"SpeedMeter\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>5</posx>\n"
            "               <posy>45</posy>\n"
            "           </widget>\n"
            "           <widget name=\"Slider\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>400</posx>\n"
            "               <posy>45</posy>\n"
            "           </widget>\n"
            "           <widget name=\"Hint\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>50</posx>\n"
            "               <posy>150</posy>\n"
            "           </widget>\n"
            "           <widget name=\"RoutingProcess\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>260</posx>\n"
            "               <posy>140</posy>\n"
            "           </widget>\n"
            "           <widget name=\"ErrorMessage\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>260</posx>\n"
            "               <posy>140</posy>\n"
            "           </widget>\n"
            "       </widgets>\n"
            "       <map>\n"
            "           <lat>51.1</lat>\n"
            "           <lon>17.03</lon>\n"
            "           <zoom>8</zoom>\n"
            "       </map>\n"
            "       <poiDisplay>\n"
            "          <poi display=\"true\" type=\"0\"/>\n"
            "          <poi display=\"false\" type=\"1\"/>\n"
            "          <poi display=\"false\" type=\"2\"/>\n"
            "          <poi display=\"false\" type=\"3\"/>\n"
            "          <poi display=\"false\" type=\"4\"/>\n"
            "          <poi display=\"false\" type=\"5\"/>\n"
            "          <poi display=\"false\" type=\"6\"/>\n"
            "          <poi display=\"false\" type=\"7\"/>\n"
            "          <poi display=\"false\" type=\"8\"/>\n"
            "          <poi display=\"false\" type=\"9\"/>\n"
            "          <poi display=\"false\" type=\"10\"/>\n"
            "          <poi display=\"false\" type=\"11\"/>\n"
            "          <poi display=\"false\" type=\"12\"/>\n"
            "          <poi display=\"false\" type=\"13\"/>\n"
            "          <poi display=\"false\" type=\"14\"/>\n"
            "          <poi display=\"false\" type=\"15\"/>\n"
            "          <poi display=\"false\" type=\"16\"/>\n"
            "          <poi display=\"false\" type=\"17\"/>\n"
            "          <poi display=\"false\" type=\"18\"/>\n"
            "          <poi display=\"false\" type=\"19\"/>\n"
            "          <poi display=\"false\" type=\"20\"/>\n"
            "          <poi display=\"false\" type=\"21\"/>\n"
            "       </poiDisplay>\n"
            "       <language>\n"
            "           <active>system</active>\n"
            "       </language>\n"
            "       <historyPoints>"
            "       </historyPoints>"
            "       <favouritePoints>"
            "       </favouritePoints>"
            "   </profile>\n"
            "</settings>\n";

    QFile file("settings.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "File not  found." << std::endl;
        return;
    }

    file.resize(0);
    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("UTF-8");
    stream << defaultXML;
    file.close();
}

void Settings::configureProfile(QString profile)
{
    //Loading core settings from xml to local variables

    QDomElement coreSettingsNode = doc->firstChildElement("settings").firstChildElement("coreSettings");
    mapPath = coreSettingsNode.firstChildElement("mapPath").text();
    mapStylePath = coreSettingsNode.firstChildElement("mapStylePath").text();
    layoutStylePath = coreSettingsNode.firstChildElement("layoutStylePath").text();
    poiFilePath = coreSettingsNode.firstChildElement("poiFilePath").text();
    poiIconsDir = coreSettingsNode.firstChildElement("poiIconsDir").text();
    simulationPath = coreSettingsNode.firstChildElement("simulationPath").text();

    QDomElement poiNode = doc->firstChildElement("settings").firstChildElement("poiSettings").firstChildElement("poi");
    while(!poiNode.isNull())
    {
        int poiType = poiNode.attributeNode("type").value().toInt();
        QString poiIconPath = poiNode.attributeNode("icon").value();
        int poiDisplayZoom = poiNode.attributeNode("zoom").value().toInt();
        PiLibocik::PoiDisplay poiDisplay(poiType, false, poiDisplayZoom, poiIconPath);
        poiDisplaySettings.insert(poiType, poiDisplay);
        poiNode = poiNode.nextSiblingElement("poi");
    }

    //Loading profile settings from xml to local variables

    QDomElement profileSettings = doc->firstChildElement("settings").firstChildElement("profile");

    //Selecting profile

    while(profileSettings.attributeNode("name").value() != profile && !profileSettings.isNull())
        profileSettings = profileSettings.nextSiblingElement();

    profileSettingsXMLNode = profileSettings;
    //When specified profile not found

    if(profileSettings.isNull())
    {
        return;
    }

    //Configure widgets settings

    QDomElement widgetSettings = profileSettings.firstChildElement("widgets").firstChildElement("widget");
    while(!widgetSettings.isNull())
    {
        QString widgetName = widgetSettings.attributeNode("name").value();
        QMap<QString,QString> widgetAttributes;
        QDomElement widgetNode = widgetSettings.firstChildElement();
        while(!widgetNode.isNull())
        {
            widgetAttributes.insert(widgetNode.nodeName(),widgetNode.text());
            widgetNode = widgetNode.nextSiblingElement();
        }
        widgetsSettings.insert(widgetName, widgetAttributes);
        widgetSettings = widgetSettings.nextSiblingElement();
    }

    //Configure plugins settings

    QDomElement pluginSettings = profileSettings.firstChildElement("plugins").firstChildElement("plugin");
    while(!pluginSettings.isNull())
    {
        QString pluginName = pluginSettings.attributeNode("name").value();
        QMap<QString,QString> pluginAttributes;
        QDomElement pluginNode = pluginSettings.firstChildElement();
        while(!pluginNode.isNull())
        {
            pluginAttributes.insert(pluginNode.nodeName(),pluginNode.text());
            pluginNode = pluginNode.nextSiblingElement();
        }
        pluginsSettings.insert(pluginName, pluginAttributes);
        pluginSettings = pluginSettings.nextSiblingElement();
    }

    //Configure map settings

    QDomElement mapSettings = profileSettings.firstChildElement("map");
    lat = mapSettings.firstChildElement("lat").text().toDouble();
    lon = mapSettings.firstChildElement("lon").text().toDouble();
    zoom = mapSettings.firstChildElement("zoom").text().toInt();

    QDomElement langSettings = profileSettings.firstChildElement("language");
    language = langSettings.firstChildElement("active").text();
    startLanguage = language.compare("System", Qt::CaseInsensitive) == 0 ? "" : language;

    //Configure poi display settings

    QDomElement poiDisplayNode = profileSettings.firstChildElement("poiDisplay").firstChildElement("poi");
    while(!poiDisplayNode.isNull())
    {
        int poiType = poiDisplayNode.attributeNode("type").value().toInt();
        bool poiDisplay = poiDisplayNode.attributeNode("display").value() == "true";
        poiDisplaySettings[poiType].setDisplay(poiDisplay);
        poiDisplayNode = poiDisplayNode.nextSiblingElement("poi");
    }

    //Load history points

    QDomElement historyPointNode = profileSettings.firstChildElement("historyPoints").firstChildElement("point");
    while(!historyPointNode.isNull())
    {
        int pos = historyPointNode.attributeNode("pos").value().toInt();
        QString name = historyPointNode.attributeNode("name").value();
        double lon = historyPointNode.attributeNode("lon").value().toDouble();
        double lat = historyPointNode.attributeNode("lat").value().toDouble();
        StorePoint sp(pos, name, lon, lat);
        historyPoints.append(sp);
        historyPointNode = historyPointNode.nextSiblingElement("point");
    }

    //Load favourite points

    QDomElement favPointNode = profileSettings.firstChildElement("favouritePoints").firstChildElement("point");
    while(!favPointNode.isNull())
    {
        int pos = favPointNode.attributeNode("pos").value().toInt();
        QString name = favPointNode.attributeNode("name").value();
        double lon = favPointNode.attributeNode("lon").value().toDouble();
        double lat = favPointNode.attributeNode("lat").value().toDouble();
        StorePoint sp(pos, name, lon, lat);
        favouritePoints.append(sp);
        favPointNode = favPointNode.nextSiblingElement("point");
    }
}

QMap<QString,QString> Settings::getWidgetSettings(QString name){
    return widgetsSettings[name];
}

void Settings::modifyWidgetSettings(QString name, QMap<QString,QString> settingsToSave)
{
    QDomElement widgetSettings = profileSettingsXMLNode.firstChildElement("widgets").firstChildElement("widget");
    while(widgetSettings.attributeNode("name").value() != name && !widgetSettings.isNull())
        widgetSettings = widgetSettings.nextSiblingElement();

    if(widgetSettings.isNull())
    {

        QDomElement el = doc->createElement("widget");
        el.setAttribute("name", name);

        QMapIterator <QString,QString> setIter(settingsToSave);
        while(setIter.hasNext()){
            setIter.next();
            QDomElement toAdd = doc->createElement(setIter.key());
            QDomText tex=doc->createTextNode(setIter.value());
            toAdd.appendChild(tex);
            el.appendChild(toAdd);
        }
        profileSettingsXMLNode.firstChildElement("widgets").appendChild(el);
    }

    for(int i = 0; i < settingsToSave.size(); i++)
    {
        QDomElement attribute = widgetSettings.firstChildElement(settingsToSave.keys().at(i));
        attribute.firstChild().toText().setData(settingsToSave.values().at(i));
    }
}

void Settings::modifyCoreSettings(QString name, QString value)
{
    QDomElement setting = doc->firstChildElement().firstChildElement("coreSettings").firstChildElement();
    while(setting.tagName() != name && !setting.isNull()){
        setting = setting.nextSiblingElement();
    }
    if(setting.isNull())
        return;

    if(setting.firstChild().isNull())
    {
        QDomText newValue = doc->createTextNode(value);
        setting.appendChild(newValue);
    }
    else
    {
        setting.firstChild().toText().setData(value);
    }

    //configureProfile(profileSettingsXMLNode.attributeNode("name").value());
}

void Settings::modifyPoiDisplaySettings(QMap<int, PiLibocik::PoiDisplay> newPoiDisplaySettings)
{
    poiDisplaySettings = newPoiDisplaySettings;
    QDomElement poiSettingsNode = profileSettingsXMLNode.firstChildElement("poiDisplay").firstChildElement("poi");
    while(!poiSettingsNode.isNull())
    {
        int type = poiSettingsNode.attributeNode("type").value().toInt();
        QString newValue = poiDisplaySettings[type].getDisplay() ? "true" : "false";
        poiSettingsNode.attributeNode("display").setValue(newValue);
        poiSettingsNode = poiSettingsNode.nextSiblingElement("poi");
    }
}

void Settings::modifyMapSettings(double lat, double lon, int zoom)
{
    this->lat=lat;
    this->lon=lon;
    this->zoom=zoom;
    QDomElement mapSettings = profileSettingsXMLNode.firstChildElement("map");
    mapSettings.firstChildElement("lat").firstChild().toText().setData(QString::number(lat));
    mapSettings.firstChildElement("lon").firstChild().toText().setData(QString::number(lon));
    mapSettings.firstChildElement("zoom").firstChild().toText().setData(QString::number(zoom));
}

void Settings::addHistoryPoint(QString name, double lon, double lat)
{
    //Search for duplicates
    foreach(StorePoint sp, historyPoints)
        if(sp.getName()==name && sp.getLon() == lon && sp.getLat() == lat)
            return;

    QDomElement historyPointsNode = profileSettingsXMLNode.firstChildElement("historyPoints");
    QDomElement newPoint = doc->createElement("point");
    newPoint.setAttribute("pos",historyPoints.size());
    newPoint.setAttribute("name", name);
    newPoint.setAttribute("lon",lon);
    newPoint.setAttribute("lat",lat);
    historyPointsNode.appendChild(newPoint);
    historyPoints.append(StorePoint(historyPoints.size(), name, lon, lat));
}

void Settings::addFavouritePoint(QString name, double lon, double lat)
{
    foreach(StorePoint sp, favouritePoints)
        if(sp.getName()==name && sp.getLon() == lon && sp.getLat() == lat)
            return;
    QDomElement favPointsNode = profileSettingsXMLNode.firstChildElement("favouritePoints");
    QDomElement newPoint = doc->createElement("point");
    newPoint.setAttribute("pos", favouritePoints.size());
    newPoint.setAttribute("name", name);
    newPoint.setAttribute("lon",lon);
    newPoint.setAttribute("lat",lat);
    favPointsNode.appendChild(newPoint);
    favouritePoints.append(StorePoint(favouritePoints.size(), name, lon, lat));
}

void Settings::removeHistoryPoint(int pos)
{

}

void Settings::removeFavouritePoint(int pos)
{
    favouritePoints.removeAt(pos);
    for(int i=0; i < favouritePoints.size(); i++){
        favouritePoints[i].setPos(i);
    }
    QDomElement favPointNode = profileSettingsXMLNode.firstChildElement("favouritePoints").firstChildElement("point");
    QDomElement pointToDel;
    int i = 0;
    while(!favPointNode.isNull())
    {
        if(i==pos)
            pointToDel=favPointNode;
        else
            favPointNode.attributeNode("pos").setValue(QString::number(i++));
        favPointNode = favPointNode.nextSiblingElement("point");
    }
    profileSettingsXMLNode.firstChildElement("favouritePoints").removeChild(pointToDel);
}

void Settings::modifyLanguageSettings()
{
    QDomElement langSettings = profileSettingsXMLNode.firstChildElement("language");

    langSettings.firstChildElement("active").firstChild().toText().setData(language);
}

void Settings::setLanguage(QString lang)
{
    language = lang;
}

QString Settings::getLanguage()
{
    QString langSymbol = languages.value(language);

    return langSymbol;
}

QString Settings::getStartLanguage()
{
    return startLanguage;
}

QString Settings::getLanguageFromLocale(QString loc)
{
    return languages.key(loc);
}

QString Settings::getLocale(QString lang)
{
    return languages.value(lang);
}

QTranslator* Settings::reloadTranslation(QString lang)
{
    QString langSymbol;

    if (lang.isEmpty()) {
        if (language.isEmpty() || language.compare("system", Qt::CaseInsensitive) == 0) {
            langSymbol = QLocale::system().name();
        } else {
            langSymbol = languages.value(language);
        }
    } else {
        langSymbol = languages.value(lang);
    }

    QString filename = "lang/" + langSymbol + ".tr";

    if (!QFile::exists(filename + ".qm")) {
        qDebug() << "File with translating doesn't exists!";
    }

    app->removeTranslator(translator);
    translator = new QTranslator;
    translator->load(filename + ".qm");

    qDebug() << "Language is settings: " << langSymbol;

    app->installTranslator(translator);

    return translator;
}


QMap<QString,QString> Settings::getPluginSettings(QString pluginName){
    QMap<QString,QString> ret=pluginsSettings[pluginName];
    return ret;
}

void Settings::modifyPluginSettings(QString name,QMap<QString,QString> settingsToSave){
    QDomElement pluginSettings = profileSettingsXMLNode.firstChildElement("plugins").firstChildElement("plugin");
    while(pluginSettings.attributeNode("name").value() != name && !pluginSettings.isNull())
        pluginSettings = pluginSettings.nextSiblingElement();

    if(pluginSettings.isNull()){
        QDomElement el = doc->createElement("plugin");
        el.setAttribute("name", name);

        QMapIterator <QString,QString> setIter(settingsToSave);
        while(setIter.hasNext()){
            setIter.next();
            QDomElement toAdd = doc->createElement(setIter.key());
            QDomText tex=doc->createTextNode(setIter.value());
            toAdd.appendChild(tex);
            el.appendChild(toAdd);
        }
        profileSettingsXMLNode.firstChildElement("plugins").appendChild(el);
    }

    for(int i = 0; i < settingsToSave.size(); i++)
    {
        QDomElement attribute = pluginSettings.firstChildElement(settingsToSave.keys().at(i));
        attribute.firstChild().toText().setData(settingsToSave.values().at(i));
    }
}
