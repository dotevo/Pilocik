#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>
#include <QDebug>
#include "twidgetmanager.h"
#include "widgets/tclockwidget.h"
#include "widgets/tspeedmeterwidget.h"
#include "maprenderwidget.h"
#include "routewindow.h"
#include "optionswindow.h"
#include "gpsinfowindow.h"
#include "settings.h"

//to delete
#include <iostream>

NavigationWindow *NavigationWindow::main=0;

NavigationWindow::NavigationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigationWindow)
{
    gps = new GPSreceiver();

    Settings::getInstance()->loadSettings();
    Settings::getInstance()->configureProfile("default");

    ui->setupUi(this);
    TWidgetManager::getInstance()->setParent(this);
    addWidgets();
    TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));
    ui->menuPanel->setVisible(false);
    addFrames();
    ui->widget->setVisible(true);
    ui->widget->lower();
}

NavigationWindow::~NavigationWindow()
{
    gps->disable();
    delete ui->widget;
    delete ui;
    delete routeWin;
    delete optionsWin;
    delete gpsInfoWin;
    delete TWidgetManager::getInstance();
    Settings::getInstance()->saveSettings();
    delete Settings::getInstance();
}

void NavigationWindow::addWidgets(){
    TWidgetManager::getInstance()->addWidget("Clock", new TClockWidget(this));
    TWidgetManager::getInstance()->addWidget("SpeedMeter", new TSpeedMeterWidget(this));
    connect(gps, SIGNAL(positionUpdate(GPSdata)), TWidgetManager::getInstance()->getWidget("SpeedMeter"), SLOT(updateSpeed(GPSdata)));
}

void NavigationWindow::addFrames(){
    routeWin=new RouteWindow(this);
    routeWin->setVisible(false);
    optionsWin=new OptionsWindow(this);
    optionsWin->setVisible(false);
    gpsInfoWin=new GPSInfoWindow(this);
    gpsInfoWin->setVisible(false);
}

void NavigationWindow::resizeEvent ( QResizeEvent * event ){
    //Przesuwanie elementow
    QSize size=this->size();
    QPoint point(0,size.height()-ui->menuButton->geometry().height());
    ui->menuButton->move(point);
    QPoint point2(75,size.height()-ui->menuButton->geometry().height());
    ui->trackingButton->move(point2);
    QPoint point3(size.width()-ui->sliderButton->geometry().width(),size.height()-ui->sliderButton->geometry().height());
    ui->sliderButton->move(point3);

    QSize menuPanelsize(size.width(),(size.height()>200)?100:size.height()/2);
    QRect rect=ui->menuPanel->geometry();
    rect.setSize(menuPanelsize);
    ui->menuPanel->setGeometry(rect);
    ui->menuPanel->raise();

    ui->widget->setSize(size);

    emit sizeChanged(this);
}

void NavigationWindow::on_menuButton_clicked(){
    ui->menuPanel->setVisible(!ui->menuPanel->isVisible());
}

void NavigationWindow::on_trackingButton_clicked(){
    ui->trackingButton->setText(ui->widget->getTracking()?"Enable tracking":"Disable tracking");
    ui->widget->setTracking(!ui->widget->getTracking());
}

void NavigationWindow::on_routeButton_clicked()
{
    ui->menuPanel->setVisible(false);
    routeWin->setVisible(true);
    routeWin->raise();
}


void NavigationWindow::on_optionsButton_clicked()
{
    ui->menuPanel->setVisible(false);
    optionsWin->setVisible(true);
    optionsWin->raise();
}

void NavigationWindow::on_gpsButton_clicked()
{
    ui->menuPanel->setVisible(false);
    gpsInfoWin->setVisible(true);
    gpsInfoWin->raise();
}

void NavigationWindow::on_zoomSlider_valueChanged(int value)
{
    ui->widget->setZoom(value);
}

void NavigationWindow::on_zoomSlider_sliderPressed()
{
    ui->widget->setStartZoom(ui->zoomSlider->value());
}

void NavigationWindow::on_zoomSlider_sliderReleased()
{
    ui->widget->setFinishZoom(ui->zoomSlider->value());
}

void NavigationWindow::on_pushButton_4_clicked()
{
    this->close();
}

