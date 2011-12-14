#include "resourcesmanager.h"
#include "filedownload.h"
#include "activesynccomm.h"

#include <QDomElement>
#include <QStringList>
#include <QDebug>
#include <QTimer>

ResourcesManager *ResourcesManager::instance = 0;

ResourcesManager* ResourcesManager::getInstance(QApplication* a)
{
    if(instance==0)
        instance=new ResourcesManager(a);
    return instance;
}

ResourcesManager::ResourcesManager(QApplication* a)
{
    app = a;
    QTimer::singleShot(100, this, SLOT(getLocalResources()));
    QTimer::singleShot(200, this, SLOT(getServerResources()));
}

void ResourcesManager::getLocalResources()
{
    localResources = new QDomDocument();
    QFile file("localresources.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    localResources->setContent(&file);
    localMaps = parseMapRes(localResources);
    localStyles = parseStyleRes(localResources);
    localPlugins = parsePluginRes(localResources);
    localApps = parseAppRes(localResources);
    emit localResObtained();
}

void ResourcesManager::getServerResources()
{
    FileDownload *res = new FileDownload();
    res->doDownload(QUrl("http://194.54.16.66/resources.xml"));
    connect(res, SIGNAL(finished(QString)), this, SLOT(storeServerRes(QString)));
}

void ResourcesManager::getDeviceResources()
{
    deviceResources = new QDomDocument();
    deviceResources->setContent(ActiveSyncComm::instance->getResources());
    deviceMaps = parseMapRes(deviceResources);
    deviceStyles = parseStyleRes(deviceResources);
    devicePlugins = parsePluginRes(deviceResources);
    deviceApps = parseAppRes(deviceResources);
    emit deviceResObtained();
}

void ResourcesManager::storeServerRes(QString res)
{
    serverResources = new QDomDocument();
    QFile file(res);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    serverResources->setContent(&file);
    serverMaps = parseMapRes(serverResources);
    serverStyles = parseStyleRes(serverResources);
    serverPlugins = parsePluginRes(serverResources);
    serverApps = parseAppRes(serverResources);
    emit serverResObtained();
}

QList<MapResource> ResourcesManager::parseMapRes(QDomDocument* resourcesXml)
{
    QDomElement mapNode = resourcesXml->firstChildElement("resources").firstChildElement("maps").firstChildElement("map");
    QList<MapResource> list;
    while(!mapNode.isNull())
    {
        MapResource map;
        map.name = mapNode.attributeNode("name").value();
        map.version = mapNode.attributeNode("version").value();
        map.size = mapNode.attributeNode("size").value().toLong();
        map.path = mapNode.attributeNode("path").value();
        QList<QPair<QString,QString> > files;
        QDomElement mapFile = mapNode.firstChildElement("file");
        while(!mapFile.isNull())
        {
            QPair<QString,QString> mf = QPair<QString,QString>(mapFile.attributeNode("name").value(),
                                                               mapFile.attributeNode("version").value());
            files.append(mf);
            mapFile = mapFile.nextSiblingElement("file");
        }
        map.files = files;
        list.append(map);
        mapNode = mapNode.nextSiblingElement("map");
    }
    return list;
}

void ResourcesManager::mapDownloaded(QString name)
{
    QDomElement map = serverResources->firstChildElement("resources").firstChildElement("maps").firstChildElement("map");
    while(!map.isNull())
    {
        if(map.attributeNode("name").value() == name)
            break;
        map = map.nextSiblingElement("map");
    }
    if(!map.isNull())
    {
        QDomElement maps = localResources->firstChildElement("resources").firstChildElement("maps");
        maps.appendChild(map);
    }
    saveLocalRes();
}

void ResourcesManager::saveLocalRes()
{
    QFile file("localresources.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    file.resize(0);
    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("UTF-8");
    localResources->save(stream,4);
    file.close();
    getLocalResources();
}

void ResourcesManager::styleDownloaded(QString name){
    QDomElement styleNode = serverResources->firstChildElement("resources").firstChildElement("styles").firstChildElement("style");
    while(!styleNode.isNull()){
        if(styleNode.attributeNode("name").value()==name){
            localResources->firstChildElement("resources").firstChildElement("styles").appendChild(styleNode);
            break;
        }
        styleNode = styleNode.nextSiblingElement("style");
    }
    saveLocalRes();
}

void ResourcesManager::pluginDownloaded(QString name){
    QDomElement pluginNode = serverResources->firstChildElement("resources").firstChildElement("plugins").firstChildElement("plugin");
    while(!pluginNode.isNull()){
        if(pluginNode.attributeNode("name").value()==name){
            localResources->firstChildElement("resources").firstChildElement("plugins").appendChild(pluginNode);
            break;
        }
        pluginNode = pluginNode.nextSiblingElement("plugin");
    }
    saveLocalRes();
}

void ResourcesManager::deviceInstall(QString appPath, QString resPath, QStringList maps, QStringList styles, QStringList plugins, QString appName, QString appArch)
{
    QString baseXML = "<?xml version='1.0' encoding='UTF-8'?>"
            "<resources>"
            "	<maps>"
            "	</maps>"
            "   <styles>"
            "   </styles>"
            "   <plugins>"
            "   </plugins>"
            "   <applications>"
            "   </applications>"
            "</resources>";
    deviceResources = new QDomDocument();
    deviceResources->setContent(baseXML);
    QDomElement map = localResources->firstChildElement("resources").firstChildElement("maps").firstChildElement("map");
    QDomElement style = localResources->firstChildElement("resources").firstChildElement("styles").firstChildElement("style");
    QDomElement plugin = localResources->firstChildElement("resources").firstChildElement("plugins").firstChildElement("plugin");
    QDomElement app = localResources->firstChildElement("resources").firstChildElement("applications").firstChildElement("application");
    while(!map.isNull()){
        if(maps.contains(map.attributeNode("name").value()))
            deviceResources->firstChildElement("resources").firstChildElement("maps").appendChild(map);
        map = map.nextSiblingElement("map");
    }
    while(!style.isNull())
    {
        if(styles.contains(style.attributeNode("name").value()))
            deviceResources->firstChildElement("resources").firstChildElement("styles").appendChild(style);
        style = style.nextSiblingElement("style");
    }
    while(!plugin.isNull())
    {
        if(plugins.contains(plugin.attributeNode("name").value()))
            deviceResources->firstChildElement("resources").firstChildElement("plugins").appendChild(plugin);
        plugin = plugin.nextSiblingElement("plugin");
    }
    while(!app.isNull())
    {
        if(appName == app.attributeNode("name").value() && appArch == app.attributeNode("architecture").value()){
            QDomNode newApp = deviceResources->firstChildElement("resources").firstChildElement("applications").appendChild(app);
            newApp.toElement().attributeNode("path").setValue(appPath);
        }
        app = app.nextSiblingElement("application");
    }
    ActiveSyncComm::getInstance()->rapiWriteFile(deviceResources->toString(), resPath);
    deviceMaps = parseMapRes(deviceResources);
    deviceStyles = parseStyleRes(deviceResources);
    devicePlugins = parsePluginRes(deviceResources);
    emit deviceResObtained();
}

void ResourcesManager::mapDeviceInstall(QString map, QString resPath){
    QDomElement mapNode = localResources->firstChildElement("resources").firstChildElement("maps").firstChildElement("map");
    while(!mapNode.isNull()){
        if(mapNode.attributeNode("name").value()==map){
            deviceResources->firstChildElement("resources").firstChildElement("maps").appendChild(mapNode);
            break;
        }
        mapNode = mapNode.nextSiblingElement("map");
    }
    ActiveSyncComm::getInstance()->rapiWriteFile(deviceResources->toString(), resPath);
    deviceMaps = parseMapRes(deviceResources);
    emit deviceResObtained();
}

void ResourcesManager::styleDeviceInstall(QString style, QString resPath){
    QDomElement styleNode = localResources->firstChildElement("resources").firstChildElement("styles").firstChildElement("style");
    while(!styleNode.isNull()){
        if(styleNode.attributeNode("name").value()==style){
            deviceResources->firstChildElement("resources").firstChildElement("styles").appendChild(styleNode);
            break;
        }
        styleNode = styleNode.nextSiblingElement("style");
    }
    ActiveSyncComm::getInstance()->rapiWriteFile(deviceResources->toString(), resPath);
    deviceStyles = parseStyleRes(deviceResources);
    emit deviceResObtained();
}

void ResourcesManager::pluginDeviceInstall(QString plugin, QString resPath){
    QDomElement pluginNode = localResources->firstChildElement("resources").firstChildElement("plugins").firstChildElement("plugin");
    while(!pluginNode.isNull()){
        if(pluginNode.attributeNode("name").value()==plugin){
            deviceResources->firstChildElement("resources").firstChildElement("plugins").appendChild(pluginNode);
            break;
        }
        pluginNode = pluginNode.nextSiblingElement("plugin");
    }
    ActiveSyncComm::getInstance()->rapiWriteFile(deviceResources->toString(), resPath);
    devicePlugins = parsePluginRes(deviceResources);
    qDebug()<<devicePlugins.size();
    emit deviceResObtained();
}

QList<FileResource> ResourcesManager::parsePluginRes(QDomDocument* resourcesXml)
{
    QDomElement pluginNode = resourcesXml->firstChildElement("resources").firstChildElement("plugins").firstChildElement("plugin");
    QList<FileResource> list;
    while(!pluginNode.isNull())
    {
        FileResource plugin;
        plugin.name = pluginNode.attributeNode("name").value();
        plugin.version = pluginNode.attributeNode("version").value();
        plugin.file = pluginNode.attributeNode("file").value();
        list.append(plugin);
        pluginNode = pluginNode.nextSiblingElement("plugin");
    }
    return list;
}

QList<FileResource> ResourcesManager::parseStyleRes(QDomDocument* resourcesXml)
{
    QDomElement styleNode = resourcesXml->firstChildElement("resources").firstChildElement("styles").firstChildElement("style");
    QList<FileResource> list;
    while(!styleNode.isNull())
    {
        FileResource style;
        style.name = styleNode.attributeNode("name").value();
        style.version = styleNode.attributeNode("version").value();
        style.file = styleNode.attributeNode("file").value();
        list.append(style);
        styleNode = styleNode.nextSiblingElement("style");
    }
    return list;
}

QList<AppResource> ResourcesManager::parseAppRes(QDomDocument* resourcesXml)
{
    QDomElement appNode = resourcesXml->firstChildElement("resources").firstChildElement("applications").firstChildElement("application");
    QList<AppResource> list;
    while(!appNode.isNull())
    {
        AppResource app;
        app.name = appNode.attributeNode("name").value();
        app.version = appNode.attributeNode("version").value();
        app.architecture = appNode.attributeNode("architecture").value();
        app.path = appNode.attributeNode("path").value();
        QList<QPair<QString,QString> > files;
        QDomElement appFile = appNode.firstChildElement("file");
        while(!appFile.isNull())
        {
            QPair<QString,QString> mf = QPair<QString,QString>(appFile.attributeNode("name").value(),
                                                               appFile.attributeNode("version").value());
            files.append(mf);
            appFile = appFile.nextSiblingElement("file");
        }
        app.files = files;
        list.append(app);
        appNode = appNode.nextSiblingElement("application");
    }
    return list;
}

QList<AppResource> ResourcesManager::appsToUpdate()
{
    QList<AppResource> toUpdate;
    foreach(AppResource dar, deviceApps)
        foreach(AppResource sar, serverApps)
            if(dar.name==sar.name && dar.architecture==sar.architecture && sar.version > dar.version){
                toUpdate.append(dar);
                break;
            }
    return toUpdate;
}

QList<MapResource> ResourcesManager::mapsToUpdate()
{
    QList<MapResource> toUpdate;
    foreach(MapResource dmr, deviceMaps)
        foreach(MapResource smr, serverMaps)
            if(dmr.name==smr.name && smr.version > dmr.version){
                toUpdate.append(dmr);
                break;
            }
    return toUpdate;
}

QList<FileResource> ResourcesManager::stylesToUpdate()
{
    QList<FileResource> toUpdate;
    foreach(FileResource dsr, deviceStyles)
        foreach(FileResource ssr, serverStyles)
            if(dsr.name==ssr.name && ssr.version > dsr.version){
                toUpdate.append(dsr);
                break;
            }
    return toUpdate;
}

QList<FileResource> ResourcesManager::pluginsToUpdate()
{
    QList<FileResource> toUpdate;
    foreach(FileResource dpr, devicePlugins)
        foreach(FileResource spr, serverPlugins)
            if(dpr.name==spr.name && spr.version > dpr.version){
                toUpdate.append(dpr);
                break;
            }
    return toUpdate;
}

