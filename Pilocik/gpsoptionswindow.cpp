#include "gpsoptionswindow.h"
#include "ui_gpsoptionswindow.h"
#include "navigationwindow.h"

GPSOptionsWindow::GPSOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::GPSOptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
}

GPSOptionsWindow::~GPSOptionsWindow()
{
    delete ui;
}

void GPSOptionsWindow::on_okButton_clicked()
{
    setVisible(false);
}
