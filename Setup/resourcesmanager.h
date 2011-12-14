#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include <QMap>
#include <QList>
#include <QPair>
#include <QObject>
#include <QDomDocument>


class ResFile
{
    QString filename;
    QString version;
    long    size;
};

class Resource
{
public:
    QString name;
    QString version;
    QString path;
    QList<ResFile> files;
};

class MapResource
{
public:
    QString name;
    QString version;
    QString path;
    long    size;
    QList< QPair<QString, QString> > files;
};

class FileResource{
public:
    QString name;
    QString version;
    QString file;
};

class AppResource
{
public:
    QString name;
    QString architecture;
    QString version;
    QString path;
    long    size;
    QList< QPair<QString, QString> > files;
};

class ResourcesManager: public QObject
{
    Q_OBJECT
public:
    static ResourcesManager* getInstance(QApplication* a = 0);
    static ResourcesManager *instance;

    void mapDownloaded(QString name);
    void styleDownloaded(QString name);
    void pluginDownloaded(QString name);

    void mapDeviceInstall(QString map, QString resPath);
    void styleDeviceInstall(QString style, QString resPath);
    void pluginDeviceInstall(QString plugin, QString resPath);
    void deviceInstall(QString appPath,
                       QString resPath,
                       QStringList maps,
                       QStringList styles,
                       QStringList plugins,
                       QString appName,
                       QString appArch);

    QList<AppResource> appsToUpdate();
    QList<MapResource> mapsToUpdate();
    QList<FileResource> stylesToUpdate();
    QList<FileResource> pluginsToUpdate();

    QList<MapResource> serverMaps;
    QList<MapResource> localMaps;
    QList<MapResource> deviceMaps;
    QList<FileResource> serverStyles;
    QList<FileResource> localStyles;
    QList<FileResource> deviceStyles;
    QList<FileResource> serverPlugins;
    QList<FileResource> localPlugins;
    QList<FileResource> devicePlugins;
    QList<AppResource> serverApps;
    QList<AppResource> localApps;
    QList<AppResource> deviceApps;

private:
    ResourcesManager(QApplication* a);
    QApplication *app;
    QDomDocument *serverResources;
    QDomDocument *localResources;
    QDomDocument *deviceResources;

    void saveLocalRes();

    QList<MapResource> parseMapRes(QDomDocument* resourcesXml);
    QList<FileResource> parseStyleRes(QDomDocument* resourcesXml);
    QList<FileResource> parsePluginRes(QDomDocument *resourcesXml);
    QList<AppResource> parseAppRes(QDomDocument* resourcesXml);

private slots:
    void storeServerRes(QString res);
    void getLocalResources();
    void getServerResources();
    void getDeviceResources();
signals:
    void serverResObtained();
    void localResObtained();
    void deviceResObtained();
};

#endif // RESOURCESMANAGER_H
