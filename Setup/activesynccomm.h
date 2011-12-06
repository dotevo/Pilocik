#ifndef ACTIVESYNCCOMM_H
#define ACTIVESYNCCOMM_H

#include <rapi.h>
#include <QString>
#include <QObject>

class ActiveSyncComm: public QObject
{
    Q_OBJECT
public:
    ~ActiveSyncComm();
    static ActiveSyncComm *getInstance(QApplication* a = 0);
    static ActiveSyncComm *instance;
    void reconnect();
    QString getDeviceSettings(bool reload=false);
    void copyToDevice(QString source, QString dest);
    void createPath(QString path);

    QString getResources();

private:
    ActiveSyncComm(QApplication *a);
    QApplication *app;
    QString settings;
    QString appPath;

    HRESULT TryRapiConnect(DWORD dwTimeOut);
    QString rapiReadFile(QString path);
    void rapiWriteFile(QString value, QString path);

signals:
    void connected();
};

#endif // ACTIVESYNCCOMM_H
