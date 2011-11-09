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
    PiLibocik::PoiFilePPOI n;
    QFile file("test.DAT");
    file.open(QIODevice::WriteOnly);
    n.saveToFile(file,pois);
    file.close();

}

MainWindow::~MainWindow()
{
    delete ui;
}
