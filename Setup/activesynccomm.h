#ifndef ACTIVESYNCCOMM_H
#define ACTIVESYNCCOMM_H

#include <rapi.h>
#include <QString>
#include <QObject>
#include <QThread>

class ActiveSyncComm: public QThread
{
    Q_OBJECT
public:
    ~ActiveSyncComm();
    static ActiveSyncComm* getInstance(QApplication* a = 0);
    static ActiveSyncComm *instance;
    QString getDeviceSettings(bool reload=false);
    void rapiDisconnect();
    void copyToDevice(QString source, QString dest);
    void createPath(QString path);
    QString getResources();
    QString rapiReadFile(QString path);
    void rapiWriteFile(QString value, QString path);
    int getStatus();

public slots:
    void reconnect();

protected:
    void run();

private:
    ActiveSyncComm(QApplication *a);
    QApplication *app;
    QString settings;
    QString appPath;
    int status;

    HRESULT TryRapiConnect(DWORD dwTimeOut);

signals:
    void statusUpdate(int status);
    void connected();
};

#endif // ACTIVESYNCCOMM_H
