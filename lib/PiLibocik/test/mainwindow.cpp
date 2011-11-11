#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pilibocik/preparedata.h>
#include <pilibocik/poi.h>
#include <pilibocik/poifileppoi.h>
#include <pilibocik/boundarybox.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/map/poland.db", "g:/poi.db", "../../PiLibocik/poiConfig.xml");

    //------------SAVE-----------------
    PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/poi.db");
    QList<PiLibocik::Poi> pois=pd->getPoiList();
    QMap<int,QString> types=pd->getPoiTypeNames();
    PiLibocik::PoiFilePPOI n;
    n.saveToFile("c:/map/mojepoi2",pois,types);

    //----------------------------------


    //-------------LOAD------------------
    //PiLibocik::PoiFilePPOI n;
    PiLibocik::BoundaryBox b(PiLibocik::Point(18.6285,54.2666),PiLibocik::Point(18.9,55));
    QList <PiLibocik::Poi> p=n.loadPOIsFromFile("c:/map/mojepoi2",b,-1);
    QListIterator <PiLibocik::Poi> iter(p);
    while(iter.hasNext()){
        PiLibocik::Poi poi=iter.next();
        qDebug()<<poi.getLat()<<":"<<poi.getLon()<<" Name:"<<poi.getName()<<" Type:"<<poi.getType();
        QList < QPair <QString,QString > > l= poi.getTags();
        QListIterator <QPair <QString,QString > > iter2(l);
        while(iter2.hasNext()){
            QPair <QString,QString > n=iter2.next();
            qDebug()<<n.first<<"="<<n.second;
        }
    }
    //-----------------------------------


    //-----BBox to geohashes------------
    /*
    PiLibocik::BoundaryBox b(PiLibocik::Point(40,40),PiLibocik::Point(50,50));
    QList <PiLibocik::Geohash> geoH=b.getGeohashesIn(2);
    QListIterator <PiLibocik::Geohash> iter(geoH);
    while(iter.hasNext()){
        PiLibocik::Geohash g=iter.next();
        qDebug()<<g.toQString();
    }*/

}

MainWindow::~MainWindow()
{
    delete ui;
}
