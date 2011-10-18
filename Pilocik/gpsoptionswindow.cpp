#include "gpsoptionswindow.h"
#include "ui_gpsoptionswindow.h"
#include "navigationwindow.h"

GPSOptionsWindow::GPSOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::GPSOptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
    ui->gpxFolderLineEdit->setText("\\ResidentFlash\\ZPI\\bielany.gps");
    gps = &(parent->gps);
}

GPSOptionsWindow::~GPSOptionsWindow()
{
    delete ui;
}

void GPSOptionsWindow::on_okButton_clicked()
{
    setVisible(false);
}

void GPSOptionsWindow::on_startSimButton_clicked()
{
    gps->setSimPath(ui->gpxFolderLineEdit->text());
    gps->setMode(gps->SIMULATION_MODE);
    gps->start();
}

void GPSOptionsWindow::on_stopSimButton_clicked()
{
    gps->disable();
}
