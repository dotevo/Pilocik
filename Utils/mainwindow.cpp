#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTime>
#include <QFileDialog>

#include <osmscout/Partitionmodel.h>
#include <osmscout/Routing.h>
#include "../lib/PiLibocik/include/pilibocik/boundarybox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();
    qApp->processEvents();

    gen = new DatabaseGen();
    part = new osmscout::Partitioning();
    gps = new GPSemulator();

    if(true) {
        osmscout::Routing *r = new osmscout::Routing();
        PiLibocik::Position p1(17.0151, 51.1234);
        PiLibocik::Position p2(17.1252, 51.1975);
        r->CalculateRoute(p1, p2);
    }

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

    connect(part, SIGNAL(initDataStatusChanged(QString)), ui->dataInitStatus, SLOT(setText(QString)));
    connect(part, SIGNAL(initDataPartProgress(int)), ui->dataInitPartialProgressBar, SLOT(setValue(int)));
    connect(part, SIGNAL(initDataOverallProgress(int)), ui->dataInitOverallProgressBar, SLOT(setValue(int)));

    connect(part, SIGNAL(prioCalcStatusChanged(QString)), ui->prioCalcStatus, SLOT(setText(QString)));
    connect(part, SIGNAL(prioCalcProgress(int)), ui->prioCalcProgressBar, SLOT(setValue(int)));

    connect(part, SIGNAL(partCalcStatusChanged(QString)), ui->partCalcStatus, SLOT(setText(QString)));
    connect(part, SIGNAL(partCalcProgress(int)), ui->partCalcProgressBar, SLOT(setValue(int)));

    connect(part, SIGNAL(initDataFinished()), this, SLOT(calculationFinished()));
    connect(part, SIGNAL(prioCalcFinished()), this, SLOT(calculationFinished()));
    connect(part, SIGNAL(partCalcFinished()), this, SLOT(calculationFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculationFinished()
{
    ui->dataInitBtn->setEnabled(true);
    ui->prioCalcButton->setEnabled(true);
    ui->partCalcButton->setEnabled(true);
}

void MainWindow::on_genBtn_clicked()
{
    gen->init(ui->osmPath->text(), ui->outputDir->text(), ui->addIdx->checkState()==Qt::Checked);
    gen->start();
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
    ui->prioCalcDataFilePath->setText(ui->dataInitOutputFilePath->text());
    ui->partCalcDataFilePath->setText(ui->dataInitOutputFilePath->text());
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
    ui->partCalcPrioFilePath->setText(ui->prioCalcOutputFilePath->text());
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

void MainWindow::on_cancelBtn_clicked()
{

}

void MainWindow::on_poiFilePathBrowseButton_clicked()
{
    ui->poiFilePath->setText(QFileDialog::getOpenFileName(this,
        tr("Chose file"), "", tr("POI Files (*.poi)"))); // TC: Nie wiem jakie jest rozszerzenie...
}

void MainWindow::on_dataInitBtn_clicked()
{
    ui->dataInitBtn->setEnabled(false);
    ui->prioCalcButton->setEnabled(false);
    ui->partCalcButton->setEnabled(false);
    part->setStage(osmscout::Partitioning::DATA_INITIALIZATION);
    part->setDatabasePath(ui->dataInitMapPath->text());
    part->setSimpleDataPath(ui->dataInitOutputFilePath->text());
    part->start();
}

void MainWindow::on_prioCalcButton_clicked()
{
    ui->dataInitBtn->setEnabled(false);
    ui->prioCalcButton->setEnabled(false);
    ui->partCalcButton->setEnabled(false);
    part->setStage(osmscout::Partitioning::PRIORITIES_CALCULATION);
    part->setSimpleDataPath(ui->prioCalcDataFilePath->text());
    part->setPrioritiesDataPath(ui->prioCalcOutputFilePath->text());
    part->start();
}

void MainWindow::on_partCalcButton_clicked()
{
    ui->dataInitBtn->setEnabled(false);
    ui->prioCalcButton->setEnabled(false);
    ui->partCalcButton->setEnabled(false);
    part->setStage(osmscout::Partitioning::PARTITIONS_CALCULATION);
    part->setSimpleDataPath(ui->partCalcDataFilePath->text());
    part->setPrioritiesDataPath(ui->partCalcPrioFilePath->text());
    part->setFinalDataPath(ui->prioCalcOutputFilePath->text());
    part->start();
}

void MainWindow::on_partBinaryRenderButton_clicked()
{
    ui->partBinaryRenderPath->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_partDatabaseRenderPathButton_clicked()
{
    ui->partDatabaseRenderPath->setText(QFileDialog::getOpenFileName(this,
        tr("Chose file"), "", tr("Database Files (*.db)")));
}

void MainWindow::on_partitionRenderButton_clicked()
{
    ui->widget->init(ui->partBinaryRenderPath->text());
}
