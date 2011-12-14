//cl activesynccomm.cpp rapi.lib /link /DELAYLOAD:rapi.dll
#include <QtGui/QApplication>
#include "mainwindow.h"
#include "windows.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    HMODULE hm = LoadLibrary(TEXT("rapi.dll"));
    if(hm)
        qDebug()<<"OK";
    else
        qDebug()<<"FAIL!!!";
//    LPTSTR path;
//    DWORD nSzie = 100;
//    GetModuleFileName(hm, path, nSzie);
//    qDebug()<<path;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
