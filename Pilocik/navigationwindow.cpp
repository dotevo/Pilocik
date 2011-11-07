#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>
#include "twidgetmanager.h"
#include "widgets/tclockwidget.h"
#include "widgets/tspeedmeterwidget.h"
#include "widgets/tsliderwidget.h"
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

    QFile layoutStyleFile(Settings::getInstance()->getLayoutStylePath());
    if (!layoutStyleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QString layoutStyle = layoutStyleFile.readAll();
    this->setStyleSheet(layoutStyle);

    mapRenderer = ui->widget;
    TWidgetManager::getInstance()->setParent(this);
    addWidgets();
    TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND));
    //TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));
    ui->menuPanel->setVisible(false);
    addFrames();
    ui->widget->setVisible(true);
    ui->widget->lower();
    ui->menuPanel->raise();
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
    TSliderWidget* slider = new TSliderWidget(this);
    slider->initZoom(Settings::getInstance()->getZoom());
    TWidgetManager::getInstance()->addWidget("Slider", slider);

    connect(gps, SIGNAL(positionUpdate(GPSdata)), TWidgetManager::getInstance()->getWidget("SpeedMeter"), SLOT(updateSpeed(GPSdata)));
    TWidgetManager::getInstance()->showAllWidgets();
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
    QPoint point2(75,size.height()-ui->trackingButton->geometry().height());
    ui->trackingButton->move(point2);
    QPoint point3(size.width()-ui->sliderButton->geometry().width(),size.height()-ui->sliderButton->geometry().height());
    ui->sliderButton->move(point3);

    QSize menuPanelsize(size.width(),(size.height()>240)?120:size.height()/2);
    QRect rect=ui->menuPanel->geometry();
    rect.setSize(menuPanelsize);
    ui->menuPanel->setGeometry(rect);
    QPoint point4(0,size.height()-menuPanelsize.height());
    ui->menuPanel->move(point4);
    ui->menuPanel->raise();

    ui->widget->setSize(size);

    emit sizeChanged(this);
}

void NavigationWindow::on_menuButton_clicked(){
    TWidgetManager::getInstance()->hideAllWidgets();
    ui->menuPanel->raise();
    ui->menuButton->raise();
    ui->menuPanel->setVisible(!ui->menuPanel->isVisible());
    if (!ui->menuPanel->isVisible()) {
        TWidgetManager::getInstance()->showAllWidgets();
    }
    ui->widget->repaint();
}

void NavigationWindow::on_trackingButton_clicked(){
    ui->trackingButton->setText(ui->widget->getTracking()?"Enable tracking":"Disable tracking");
    ui->widget->setTracking(!ui->widget->getTracking());
}

void NavigationWindow::on_routeButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    routeWin->setVisible(true);
    routeWin->raise();
    connect(routeWin, SIGNAL(windowClosed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_optionsButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    optionsWin->setVisible(true);
    optionsWin->raise();
    connect(optionsWin, SIGNAL(windowClosed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_gpsButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    gpsInfoWin->setVisible(true);
    gpsInfoWin->raise();
    connect(gpsInfoWin, SIGNAL(windowClosed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_exitButton_clicked() {
    setVisible(false);
    this->close();
}

void NavigationWindow::menuClosedSlot() {
    ui->menuButton->setVisible(true);
    ui->trackingButton->setVisible(true);
    ui->sliderButton->setVisible(true);
    ui->menuPanel->setVisible(true);
}

void NavigationWindow::on_sliderButton_clicked() {
    TWidgetManager::getInstance()->changeMode();
    ui->sliderButton->setText(ui->sliderButton->text().compare("<--->") ? "<--->" : "<>");
}
