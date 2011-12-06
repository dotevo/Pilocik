#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pilibocik/preparedata.h>
#include <pilibocik/poi.h>
#include <pilibocik/poifileppoi.h>
#include <pilibocik/boundarybox.h>
#include <QDebug>
#include <QTime>
#include <pilibocik/geohash.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("g:/_poland.db","../config.xml");

    //------------SAVE-----------------
//    PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("g:/poi.db");
    qDebug()<<"Generating binary files";
    QList<PiLibocik::Poi> pois=pd->getPoiList();
    QMap<int,QString> types=pd->getPoiTypeNames();
    PiLibocik::PoiFilePPOI n;
    n.saveToFile("g:/poi",pois,types);

    //----------------------------------


    //-------------LOAD------------------
//    PiLibocik::PoiFilePPOI n;
//    PiLibocik::BoundaryBox b(PiLibocik::Position(17,51),PiLibocik::Position(17.1,51.1));
//    QTime t;
//    t.start();
//    QList <PiLibocik::Poi> p=n.loadPOIsFromFile("G:/mojepoi2",b,-1);
//    QListIterator <PiLibocik::Poi> iter(p);
//    int i = 0;
//    while(iter.hasNext()){
//        PiLibocik::Poi poi=iter.next();
//        i++;
//        qDebug()<<poi.getLat()<<":"<<poi.getLon()<<" Name:"<<poi.getName()<<" Type:"<<poi.getType()<<"Geohash:"<<poi.getGeohash();
//        QList < QPair <QString,QString > > l= poi.getTags();
//        QListIterator <QPair <QString,QString > > iter2(l);
//        while(iter2.hasNext()){
//            QPair <QString,QString > n=iter2.next();
////            qDebug()<<n.first<<"="<<n.second;
//        }
//    }
//    qDebug()<<"Time: "<<t.elapsed()<<"Size: "<<i;
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



    // POI SAVE
//        QList<PiLibocik::Poi> pois;
//        QList < QPair <QString, QString> >  tags;
//        tags.append(QPair<QString,QString>("name",QString::fromUtf8("ąKłę")));
//        QString geo1=PiLibocik::Geohash::generateGeohash(20,10,5);
//        PiLibocik::Poi poi(20,10,QString::fromUtf8("ąasdęś"),0,tags,geo1);
//        qDebug()<<"POI:"<<poi.getName();
//        pois.append(poi);
//        QString geo2=PiLibocik::Geohash::generateGeohash(19.1,10.0,5);
//        PiLibocik::Poi poi2(10.1,10,QString::fromUtf8("Kąpłęta"),0,tags,geo2);
//        pois.append(poi2);
//        QMap<int,QString> types;
//        types.insert(0,QString::fromUtf8("ędłę"));
//        PiLibocik::PoiFilePPOI ppo;
//        ppo.saveToFile("poi",pois,types);

//        ui->label->setText("");
//            PiLibocik::PoiFilePPOI n;
//            PiLibocik::BoundaryBox b(PiLibocik::Position(20,10),PiLibocik::Position(20,10));
//            QTime t;
//            t.start();
//            QList <PiLibocik::Poi> p=n.loadPOIsFromFile("poi",b,-1);
//            QListIterator <PiLibocik::Poi> iter(p);
//            int i = 0;
//            while(iter.hasNext()){
//                PiLibocik::Poi poi=iter.next();
//                i++;
//                ui->label->setText(ui->label->text()+":"+poi.getName());
//                QList < QPair <QString,QString > > l= poi.getTags();
//                QListIterator <QPair <QString,QString > > iter2(l);
//                while(iter2.hasNext()){
//                    QPair <QString,QString > n=iter2.next();
//                    ui->label->setText(ui->label->text()+"->"+n.first+"="+n.second);
//                    qDebug()<<n.first<<"="<<n.second;
//                }
//            }
//            qDebug()<<"Time: "<<t.elapsed()<<"Size: "<<i;

}

MainWindow::~MainWindow()
{
    delete ui;
}
