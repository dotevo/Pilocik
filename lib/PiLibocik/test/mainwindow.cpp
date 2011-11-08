#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <pilibocik/preparedata.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("c:/map/poland.db", "g:/poi.db", "../../PiLibocik/poiConfig.xml");
    PiLibocik::PrepareData* pd = new PiLibocik::PrepareData("g:/poi.db");
}

MainWindow::~MainWindow()
{
    delete ui;
}
