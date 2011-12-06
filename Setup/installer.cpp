#include "installer.h"
#include "activesynccomm.h"
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTimer>

Installer::Installer(int dev)
{
    device = dev;
}

void Installer::init(QString p, QList<MapResource> maps)
{
    path = p+"\\";
    this->maps = maps;
    paths.append(path);
    paths.append(path+"map");
    foreach(MapResource map, maps)
    {
        QString mapPath = path+map.path;
        paths.append(mapPath);
        typedef QPair<QString, QString> filePair;
        foreach(filePair f, map.files){
            copyBuffer.append(QPair<QString, QString>("resources\\"+map.path+f.first, mapPath+f.first));
        }
    }
    if(device==0)
    {
        foreach(AppResource app, ResourcesManager::getInstance()->localApps)
        {
            if(app.name == "Pilocik" && app.architecture=="winCE")
            {
                typedef QPair<QString, QString> filePair;
                foreach(filePair f, app.files)
                {
                    if(f.first=="settings.xml")
                        prepareSettings("resources/"+app.path+"settings.xml");
                    copyBuffer.append(filePair("resources/"+app.path+f.first, path+f.first));
                }
                break;
            }
        }
    }
}

void Installer::prepareSettings(QString setPath)
{
    QDomDocument *doc = new QDomDocument();
    QFile file(setPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    doc->setContent(&file);
    file.close();
    modifyCoreSettings(doc,"mapPath", path+maps.at(0).path);
    modifyCoreSettings(doc,"mapStylePath", path+maps.at(0).path+"standard.oss.xml");
    modifyCoreSettings(doc,"layoutStylePath", path+"defaultLayoutStyle.qss");
    modifyCoreSettings(doc,"poiFilePath", path+maps.at(0).path+"poi");
    modifyCoreSettings(doc,"poiIconsDir", path+maps.at(0).path);
    QFile file2(setPath);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    file2.resize(0);
    QTextStream stream;
    stream.setDevice(&file2);
    stream.setCodec("UTF-8");
    doc->save(stream,4);
    file2.close();
}

void Installer::modifyCoreSettings(QDomDocument *doc, QString name, QString value)
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
}

void Installer::install()
{
    foreach(QString p, paths)
    {
        ActiveSyncComm::getInstance()->createPath(p);
    }

    typedef QPair<QString, QString> copyPair;
    int i=0;
    foreach(copyPair cp, copyBuffer)
    {
        ActiveSyncComm::getInstance()->copyToDevice(cp.first, cp.second);
        qDebug()<<cp.first<<cp.second;
        emit progressUpdate(++i*100/copyBuffer.size());
    }
}

void Installer::run()
{
    install();
}
