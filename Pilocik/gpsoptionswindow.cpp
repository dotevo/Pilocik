#include "gpsoptionswindow.h"
#include "ui_gpsoptionswindow.h"

GPSOptionsWindow::GPSOptionsWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GPSOptionsWindow)
{
    ui->setupUi(this);
}

GPSOptionsWindow::~GPSOptionsWindow()
{
    delete ui;
}
