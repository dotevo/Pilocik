#include <QtGui/QApplication>
#include <QDebug>
#include "mainwindow.h"
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/way.h>
#include <pilibocik/partition/edge.h>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>
#include <pilibocik/poifileppoi.h>
#include <pilibocik/geohash.h>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
