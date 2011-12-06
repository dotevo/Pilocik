#ifndef RESOURCESMANAGER_H
#define RESOURCESMANAGER_H

#include <QMap>
#include <QList>
#include <QPair>
#include <QObject>
#include <QDomDocument>

class MapResource
{
public:
    QString name;
    QString version;
    QString path;
    long    size;
    QList< QPair<QString, QString> > files;
};

class StyleResource
{
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

    QList<MapResource> serverMaps;
    QList<MapResource> localMaps;
    QList<MapResource> deviceMaps;
    QList<StyleResource> serverStyles;
    QList<StyleResource> localStyles;
    QList<StyleResource> deviceStyles;
    QList<AppResource> serverApps;
    QList<AppResource> localApps;
    QList<AppResource> deviceApps;

private:
    ResourcesManager(QApplication* a);
    QApplication *app;
    QDomDocument *serverResources;
    QDomDocument *localResources;
    QDomDocument *deviceResources;

    QList<MapResource> parseMapRes(QDomDocument* resourcesXml);
    QList<StyleResource> parseStyleRes(QDomDocument* resourcesXml);
    QList<AppResource> parseAppRes(QDomDocument* resourcesXml);

private slots:
    void storeServerRes(QString res);
    void getLocalResources();
    void getServerResources();
    void getDeviceResources();
signals:
    void serverResObtained();
    void localResObtained();
};

#endif // RESOURCESMANAGER_H
