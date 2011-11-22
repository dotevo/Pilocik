#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include "settings.h"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QTranslator>

Settings * Settings::instance=0;

Settings::Settings(QApplication* a)
{
    app = a;

    addLanguages();

    startLanguage = "";
}

Settings::~Settings()
{
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
            "       <simulationPath></simulationPath>\n"
            "   </coreSettings>\n"
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
            "           <qidget name=\"Hint\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>50</posx>\n"
            "               <posy>150</posy>\n"
            "           </widget>\n"
            "           <qidget name=\"RoutingProcess\">\n"
            "               <enabled>true</enabled>\n"
            "               <posx>10</posx>\n"
            "               <posy>10</posy>\n"
            "           </widget>\n"
            "       </widgets>\n"
            "       <map>\n"
            "           <lat>51.1</lat>\n"
            "           <lon>17.03</lon>\n"
            "           <zoom>8</zoom>\n"
            "       </map>\n"
            "       <language>\n"
            "           <active>system</active>\n"
            "       </language>\n"
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
    simulationPath = coreSettingsNode.firstChildElement("simulationPath").text();

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

    //Configure map settings

    QDomElement mapSettings = profileSettings.firstChildElement("map");
    lat = mapSettings.firstChildElement("lat").text().toDouble();
    lon = mapSettings.firstChildElement("lon").text().toDouble();
    zoom = mapSettings.firstChildElement("zoom").text().toInt();

    QDomElement langSettings = profileSettings.firstChildElement("language");
    language = langSettings.firstChildElement("active").text();
    startLanguage = language.compare("System", Qt::CaseInsensitive) == 0 ? "" : language;
}

QMap<QString,QString> Settings::getWidgetSettings(QString name)
{
    return widgetsSettings[name];
}

void Settings::modifyWidgetSettings(QString name, QMap<QString,QString> settingsToSave)
{
    QDomElement widgetSettings = profileSettingsXMLNode.firstChildElement("widgets").firstChildElement("widget");
    while(widgetSettings.attributeNode("name").value() != name && !widgetSettings.isNull())
        widgetSettings = widgetSettings.nextSiblingElement();

    if(widgetSettings.isNull())
    {
        return;
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

void Settings::modifyMapSettings(double lat, double lon, int zoom)
{
    this->lat=lat;
    this->lon=lon;
    this->zoom=zoom;
    //QDomElement mapSettings = doc->firstChildElement("settings").firstChildElement("map");
    QDomElement mapSettings = profileSettingsXMLNode.firstChildElement("map");
    mapSettings.firstChildElement("lat").firstChild().toText().setData(QString::number(lat));
    mapSettings.firstChildElement("lon").firstChild().toText().setData(QString::number(lon));
    mapSettings.firstChildElement("zoom").firstChild().toText().setData(QString::number(zoom));
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



