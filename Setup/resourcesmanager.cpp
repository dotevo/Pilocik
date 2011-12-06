#include "resourcesmanager.h"
#include "filedownload.h"
#include "activesynccomm.h"

#include <QDomElement>
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
    deviceApps = parseAppRes(deviceResources);
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

QList<StyleResource> ResourcesManager::parseStyleRes(QDomDocument* resourcesXml)
{
//    QDomElement styleNode = resourcesXml->firstChildElement("resources").firstChildElement("styles").firstChildElement("style");
    QList<StyleResource> list;
//    while(!styleNode.isNull())
//    {
//        StyleResource style;
//        style.name = styleNode.attributeNode("name").value();
//        style.version = styleNode.attributeNode("version").value();
//        style.file = styleNode.attributeNode("file").value();
//        list.append(style);
//        styleNode = appNode.nextSiblingElement("application");
//    }
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

