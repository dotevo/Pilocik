#include "activesynccomm.h"
#include <QDebug>
#include <QString>

ActiveSyncComm::ActiveSyncComm()
{
    HRESULT hr;

    hr = TryRapiConnect(5000);
    if( !SUCCEEDED(hr) )
    {
        qDebug()<<"Could not find device";
    } else {
        appPath = rapiReadFile("\\ResidentFlash\\Pilocik.info");
        qDebug()<<appPath;
    }
}

ActiveSyncComm::~ActiveSyncComm()
{
     CeRapiUninit();
}

void ActiveSyncComm::setInstallPath(QString path)
{
    rapiWriteFile(path, "\\ResidentFlash\\Pilocik.info");
    appPath = path;
}

QString ActiveSyncComm::getInstallPath()
{
    return appPath;
}

QString ActiveSyncComm::getDeviceSettings(bool reload)
{
    if(settings.isEmpty() || reload){
        settings.clear();
        rapiReadFile(appPath+"\\settings.xml");
    }
    return settings;
}



HRESULT ActiveSyncComm::TryRapiConnect(DWORD dwTimeOut)
{
    HRESULT            hr = E_FAIL;
    RAPIINIT           riCopy;
    bool          fInitialized = false;

    ZeroMemory(&riCopy, sizeof(riCopy));
    riCopy.cbSize = sizeof(riCopy);

    hr = CeRapiInitEx(&riCopy);
    if (SUCCEEDED(hr))
    {
        DWORD dwRapiInit = 0;
        fInitialized = true;

        dwRapiInit = WaitForSingleObject(
                    riCopy.heRapiInit,
                    dwTimeOut);
        if (WAIT_OBJECT_0 == dwRapiInit)
        {
            // heRapiInit signaled:
            // set return error code to return value of RAPI Init function
            hr = riCopy.hrRapiInit;
        }
        else if (WAIT_TIMEOUT == dwRapiInit)
        {
            // timed out: device is probably not connected
            // or not responding
            hr = HRESULT_FROM_WIN32(ERROR_TIMEOUT);
        }
        else
        {
            // WaitForSingleObject failed
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }

   if (fInitialized && FAILED(hr))
   {
        CeRapiUninit();
   }
    return hr;
}

QString ActiveSyncComm::rapiReadFile(QString path){
    HANDLE hFile;
    DWORD  dwBytesRead;
    BYTE   buff[4096];
    QString output;
    const wchar_t * src = reinterpret_cast<const wchar_t *>(path.utf16());

    // Open the existing file.

    hFile = CeCreateFile(src,
            GENERIC_READ,             // open for reading
            FILE_SHARE_READ,          // do not share
            NULL,                     // no security
            OPEN_EXISTING,            // existing file only
            FILE_ATTRIBUTE_NORMAL,    // normal file
            NULL);                    // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
     qDebug()<<"Could not open";
     return "";
    }

    while (CeReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL)
         && dwBytesRead > 0)
    {
        QByteArray qba = QByteArray::fromRawData((char*)buff, dwBytesRead);
        output.append(qba);
    }

    CeCloseHandle(hFile);
    return output;
}

void ActiveSyncComm::rapiWriteFile(QString value, QString path){
    HANDLE hFile;
    DWORD  dwBytesWrite;

    QByteArray enc=value.toUtf8();
    char *s=new char[enc.size()+1];
    strcpy(s,enc.data());

    const wchar_t * dst = reinterpret_cast<const wchar_t *>(path.utf16());

    hFile = CeCreateFile(dst,
            GENERIC_WRITE,             // open for reading
            FILE_SHARE_READ,          // do not share
            NULL,                     // no security
            CREATE_ALWAYS,              // existing file only
            FILE_ATTRIBUTE_NORMAL,    // normal file
            NULL);                    // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
         qDebug()<<"Could not open";
         return;
    }

    CeWriteFile(hFile, s, value.length(), &dwBytesWrite, NULL);
    CeCloseHandle(hFile);
}

void ActiveSyncComm::copyToDevice(QString source, QString dest)
{
    const wchar_t * src = reinterpret_cast<const wchar_t *>(source.utf16());
    const wchar_t * dst = reinterpret_cast<const wchar_t *>(dest.utf16());


    HANDLE hSrc, hDst;
    DWORD  dwBytesRead, dwBytesWrite;
    BYTE   buff[4096];

    hSrc = CreateFile(src,
            GENERIC_READ,             // open for reading
            FILE_SHARE_READ,          // do not share
            NULL,                     // no security
            OPEN_EXISTING,              // existing file only
            FILE_ATTRIBUTE_NORMAL,    // normal file
            NULL);                    // no attr. template

    if (hSrc == INVALID_HANDLE_VALUE)
    {
         qDebug()<<"Could not open source";
         return;
    }

    hDst = CeCreateFile(dst,
            GENERIC_WRITE,             // open for reading
            FILE_SHARE_READ,          // do not share
            NULL,                     // no security
            CREATE_ALWAYS,              // existing file only
            FILE_ATTRIBUTE_NORMAL,    // normal file
            NULL);                    // no attr. template

    if (hSrc == INVALID_HANDLE_VALUE)
    {
         qDebug()<<"Could not open destination";
         return;
    }

    do{
        if(ReadFile(hSrc,&buff,sizeof(buff),&dwBytesRead,NULL))
        {
            if(!CeWriteFile(hDst, &buff, dwBytesRead, &dwBytesWrite, NULL))
                goto bailOut;
        }
        else
            goto bailOut;
    } while(dwBytesRead);

    bailOut:
    if(hSrc)
    {
        CloseHandle(hSrc);
        hSrc = NULL;
    }
    if(hDst)
    {
        CeCloseHandle(hDst);
        hDst = NULL;
    }
}
