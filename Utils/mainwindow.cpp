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

    currentTab = 0;

    updateTab();

    if(false) {
        //osmscout::Routing *r = new osmscout::Routing();
        //PiLibocik::Position p1(17.0151, 51.1234);
        //PiLibocik::Position p2(17.1252, 51.1975);
        //QList< osmscout::Routing::Step > route = r->CalculateRoute(p1, p2);
        /*QListIterator< osmscout::Routing::Step > it(route);
        while(it.hasNext()) {
            osmscout::Routing::Step s = it.next();
            qDebug() << s.lon << ":" << s.lat << ((s.crossing) ? " x" : "");
        }*/
    }

    Qt::WindowFlags flags = Qt::FramelessWindowHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground);

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

void MainWindow::updateTab()
{
    ui->tabWidget->setCurrentIndex(currentTab);

    QString activeTabStyle("background: qradialgradient(cx:0.5, cy:0.5, radius: 1,fx:0.5, fy:0.5,stop:0 #3c434d,stop:1 #45484d); color: white; border: 0; border-top-right-radius: 0px; border-bottom-right-radius: 0px; padding: 2px; font-weight: bold; border-radius: 5px; border-right: 0; height: 56px; width: 130px;");
    QString inactiveTabStyle("background: qradialgradient(cx:0.5, cy:0.5, radius: 1,fx:0.5, fy:0.5,stop:0 #ffffff,stop:1 #bbbbbb); color: orange; border: 1px solid darkorange; height: 56px; width: 130px;");

    ui->tabMapGenButton->setStyleSheet(activeTabStyle);
    ui->tabPartGenButton->setStyleSheet(activeTabStyle);
    ui->tabPartRenderButton->setStyleSheet(activeTabStyle);
    ui->tabSimButton->setStyleSheet(activeTabStyle);

    switch(currentTab) {
    case 0:
        ui->tabMapGenButton->setStyleSheet(inactiveTabStyle);
        break;
    case 1:
        ui->tabSimButton->setStyleSheet(inactiveTabStyle);
        break;
    case 2:
        ui->tabPartGenButton->setStyleSheet(inactiveTabStyle);
        break;
    case 3:
        ui->tabPartRenderButton->setStyleSheet(inactiveTabStyle);
        break;
    default:
        break;
    }
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
    part->setCoordinates(ui->minLonInput->value(), ui->maxLonInput->value(), ui->minLatInput->value(), ui->maxLatInput->value());
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
    part->setFinalDataPath(ui->partCalcOutputPath->text());
    part->setPrecision(ui->precisionInput->value());
    part->start();
}

void MainWindow::on_partBinaryRenderButton_clicked()
{
    ui->partBinaryRenderPath->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_partitionRenderButton_clicked()
{
    ui->widget->init(ui->partBinaryRenderPath->text());
}

void MainWindow::on_hideButton_clicked()
{
    showMinimized();
}

void MainWindow::on_closeButton_clicked()
{
    setVisible(false);
    this->close();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
            (event->globalY() - geometry().y()) < 103 &&
            (event->globalX() - geometry().x()) < 692) {
        isDraging = true;
        dragPosition = event->globalPos() - QPoint(geometry().x(), geometry().y());
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDraging && event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isDraging = false;
}

void MainWindow::on_tabMapGenButton_clicked()
{
    currentTab = 0;
    updateTab();
}

void MainWindow::on_tabSimButton_clicked()
{
    currentTab = 1;
    updateTab();
}

void MainWindow::on_tabPartGenButton_clicked()
{
    currentTab = 2;
    updateTab();
}

void MainWindow::on_tabPartRenderButton_clicked()
{
    currentTab = 3;
    updateTab();
}
