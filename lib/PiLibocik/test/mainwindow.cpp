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

    /*PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/poi.db");
    QList<PiLibocik::Poi> pois=pd->getPoiList();
    QMap<int,QString> types=pd->getPoiTypeNames();*/
    PiLibocik::PoiFilePPOI n;
    //n.saveToFile("c:/map/mojepoi2",pois,types);

    //PiLibocik::PoiFilePPOI n;
    PiLibocik::BoundaryBox b(PiLibocik::Point(10,10),PiLibocik::Point(100,100));
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

}

MainWindow::~MainWindow()
{
    delete ui;
}
