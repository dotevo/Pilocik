#ifndef ACTIVESYNCCOMM_H
#define ACTIVESYNCCOMM_H

#include <rapi.h>
#include <QString>

class ActiveSyncComm
{
public:
    ActiveSyncComm();
    ~ActiveSyncComm();
    QString getDeviceSettings(bool reload=false);
    void copyToDevice(QString source, QString dest);

    void setInstallPath(QString path);
    QString getInstallPath();

private:
    QString settings;
    QString appPath;

    HRESULT TryRapiConnect(DWORD dwTimeOut);
    QString rapiReadFile(QString path);
    void rapiWriteFile(QString value, QString path);
};

#endif // ACTIVESYNCCOMM_H
