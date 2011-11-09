#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pilibocik/preparedata.h>
#include <pilibocik/poi.h>
#include <pilibocik/poifileppoi.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/map/poland.db", "g:/poi.db", "../../PiLibocik/poiConfig.xml");

    PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/poi.db");
    QList<PiLibocik::Poi> pois=pd->getPoiList();
    /*for(int i=0;i<pois.count();i++){
        PiLibocik::Poi p=pois.at(i);
        qDebug()<<p.getLat();
    }*/

    QMap<int,QString> types=pd->getPoiTypeNames();
    PiLibocik::PoiFilePPOI n;
    n.saveToFile("c:/map/mojepoi2",pois,types);

    //PiLibocik::PoiFilePPOI n;
    //QList<PiLibocik::Poi> pois=n.loadFromFile("c:/map/mojepoi",poi)

}

MainWindow::~MainWindow()
{
    delete ui;
}
