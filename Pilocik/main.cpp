#include <QtGui/QApplication>
#include "navigationwindow.h"
#include "pluginmanager.h"
#include <QtGlobal>
#include <QDebug>
#include <QFile>
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication *a = new QApplication(argc, argv);

    Settings::getInstance(a)->loadSettings();
    Settings::getInstance()->configureProfile("default");

    Settings::getInstance()->reloadTranslation();

    NavigationWindow w;
    NavigationWindow::main=&w;
    w.show();

    PluginManager::getInstance()->initAll();
    PluginManager::getInstance()->runAll();

    Settings::getInstance()->reloadTranslation(Settings::getInstance()->getStartLanguage());

    //Dla windowsa CE fullscreen
    #ifdef Q_OS_WINCE_STD
        w.showFullScreen();
    #endif

    return a->exec();
}

