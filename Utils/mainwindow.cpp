#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTime>

#include <osmscout/Partitioning.h>
#include <osmscout/Partitionmodel.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();
    qApp->processEvents();

    gen = new DatabaseGen();
    gps = new GPSemulator();

    connect(gen, SIGNAL(progressUpdate(int)), ui->mcProgressBar, SLOT(setValue(int)));
    connect(gen, SIGNAL(statusUpdate(QString)), ui->mcStatus, SLOT(setText(QString)));

    connect(ui->zoom, SIGNAL(valueChanged(int)), ui->widget, SLOT(changeZoom(int)));
    connect(ui->showNodes, SIGNAL(stateChanged(int)), ui->widget, SLOT(showNodesChange(int)));
    connect(ui->showWays, SIGNAL(stateChanged(int)),ui->widget, SLOT(showWaysChange(int)));
    connect(ui->showBoundaryEdges, SIGNAL(stateChanged(int)), ui->widget, SLOT(showBoundaryEdges(int)));

    connect(gps, SIGNAL(dataSend(QString)), ui->simOutput, SLOT(setText(QString)));
    connect(gps, SIGNAL(progressUpdate(int)), ui->simSlider, SLOT(setValue(int)));
    connect(ui->simSpeed, SIGNAL(valueChanged(double)), gps, SLOT(setSpeed(double)));
    connect(ui->simSlider, SIGNAL(valueChanged(int)), gps, SLOT(selectSimPoint(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_genBtn_clicked()
{
    gen->init(ui->osmPath->text(), ui->outputDir->text(), ui->addIdx->checkState()==Qt::Checked);
    gen->start();
}

void MainWindow::on_partitionOk_clicked()
{
    ui->widget->init(ui->partRenderPath->text());
}

void MainWindow::on_simStartBtn_clicked()
{
    if(!gps->isRunning()&&!gps->isPaused())
        gps->init(ui->simPath->text());
    gps->start();
}

void MainWindow::on_simStopBtn_clicked()
{
    gps->stop();
    ui->simOutput->setText("");
}

void MainWindow::on_simPauseBtn_clicked()
{
    gps->pause();
}

void MainWindow::on_partGenBtn_clicked()
{
    //osmscout::Partitioning part(ui->partGenMapPath->text(), ui->partGenMapStylePath->text() );
    osmscout::Partitioning part;
    //part.TestAlgorithm();

    osmscout::Partitioning::DatabasePartition dbPart=part.FindPartition();

    part.saveToDatabase(ui->partGenOutputPath->text(),dbPart);
}
