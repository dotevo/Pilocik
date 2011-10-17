#include <QtGui/QApplication>
#include "navigationwindow.h"
#include <QtGlobal>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NavigationWindow w;
    NavigationWindow::main=&w;
    w.show();

    //Dla windowsa CE fullscreen
    #ifdef Q_OS_WINCE_STD
        w.showFullScreen();
    #endif

    return a.exec();
}
