#ifndef INSTALLER_H
#define INSTALLER_H

#include "resourcesmanager.h"

#include <QThread>
#include <QList>
#include <QStringList>
#include <QDomDocument>

class Installer: public QThread
{
    Q_OBJECT
public:
    Installer(int dev=0);
    void init(QString path, QList<MapResource> maps);
    void install();

private:
    int device;
    QString path;
    QStringList paths;
    QList<MapResource> maps;
    QList<QPair<QString, QString> > copyBuffer;

    void prepareSettings(QString setPath);
    void modifyCoreSettings(QDomDocument *doc, QString name, QString value);

protected:
    void run();

signals:
    void progressUpdate(int percent);
};

#endif // INSTALLER_H
