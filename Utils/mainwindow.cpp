#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTime>
#include <QFileDialog>

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

void MainWindow::on_dataInitOutputFilePathButton_clicked()
{
    ui->dataInitOutputFilePath->setText(QFileDialog::getSaveFileName(this,
         tr("Chose file"), "", tr("Text Files (*.txt)")));
}

void MainWindow::on_dataInitMapPathButton_clicked()
{
    ui->dataInitMapPath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("Database Files (*.db)")));
}

void MainWindow::on_prioCalcDataFilePathButton_clicked()
{
    ui->prioCalcDataFilePath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("Text Files (*.txt)")));
}

void MainWindow::on_prioCalcOutputFilePathButton_clicked()
{
    ui->prioCalcOutputFilePath->setText(QFileDialog::getSaveFileName(this,
         tr("Chose file"), "", tr("Text Files (*.txt)")));
}

void MainWindow::on_partCalcDataFilePathButton_clicked()
{
    ui->partCalcDataFilePath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("Text Files (*.txt)")));
}

void MainWindow::on_partCalcPrioFilePathButton_clicked()
{
    ui->partCalcPrioFilePath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("Text Files (*.txt)")));
}

void MainWindow::on_partCalcOutputPathButton_clicked()
{
    ui->partCalcOutputPath->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_dataInitBtn_clicked()
{
    osmscout::Partitioning part;

    part.InitData();
    part.SaveData(ui->dataInitOutputFilePath->text());
}

void MainWindow::on_osmPathButton_clicked()
{
    ui->osmPath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("OSM Files (*.osm)")));
}

void MainWindow::on_outputDirButton_clicked()
{
    ui->outputDir->setText(QFileDialog::getSaveFileName(this,
         tr("Chose file"), "", tr("Database Files (*.db)")));
}

void MainWindow::on_simPathButton_clicked()
{
    ui->simPath->setText(QFileDialog::getOpenFileName(this,
         tr("Chose file"), "", tr("GPS Files (*.gps)")));
}
