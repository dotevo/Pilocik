#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>
#include "twidgetmanager.h"
#include "widgets/tclockwidget.h"
#include "widgets/tspeedmeterwidget.h"
#include "widgets/tsliderwidget.h"
#include "widgets/thandymenuwidget.h"
#include "widgets/thintwidget.h"
#include "widgets/troutingprogresswidget.h"
#include "widgets/terrorwidget.h"
#include "widgets/tpoiinfowidget.h"
#include "maprenderwidget.h"
#include "routewindow.h"
#include "optionswindow.h"
#include "gpsinfowindow.h"
#include "settings.h"
#include "QDebug"

//to delete
#include <iostream>

NavigationWindow *NavigationWindow::main=0;

NavigationWindow::NavigationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigationWindow)
{
    mapRenderer =0;
    gps = new GPSreceiver();

    ui->setupUi(this);

    mapRenderer = ui->widget;

    TWidgetManager::getInstance()->setParent(this);
    addWidgets();
    TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND));
    //TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));

    mapRenderer->updateHint();

    ui->menuPanel->setVisible(false);
    addFrames();
    ui->widget->setVisible(true);
    ui->widget->lower();
    ui->menuPanel->raise();

    QFile layoutStyleFile(Settings::getInstance()->getLayoutStylePath());
    if (!layoutStyleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QString layoutStyle = layoutStyleFile.readAll();
    this->setStyleSheet(layoutStyle);
    layoutStyleFile.close();
}

NavigationWindow::~NavigationWindow()
{    
    gps->disable();
    //delete ui->widget;
    //delete ui;
    delete routeWin;
    delete optionsWin;
    delete gpsInfoWin;
    delete mapRenderer;
    delete TWidgetManager::getInstance();
    Settings::getInstance()->saveSettings();
    delete Settings::getInstance();
}

void NavigationWindow::addWidgets(){
    TWidgetManager::getInstance()->addWidget("Hint", new THintWidget(this));
    TWidgetManager::getInstance()->addWidget("Clock", new TClockWidget(this));
    TWidgetManager::getInstance()->addWidget("SpeedMeter", new TSpeedMeterWidget(this));
    TSliderWidget* slider = new TSliderWidget(this);
    slider->initZoom(Settings::getInstance()->getZoom());
    TWidgetManager::getInstance()->addWidget("Slider", slider);

    // not always visible
    TWidgetManager::getInstance()->addWidget("RoutingProgress", new TRoutingProgressWidget(this));
    TWidgetManager::getInstance()->getWidget("RoutingProgress")->setVisible(false);
    TWidgetManager::getInstance()->addWidget("ErrorMessage", new TErrorWidget(this));
    TWidgetManager::getInstance()->getWidget("ErrorMessage")->setVisible(false);
    TWidgetManager::getInstance()->addWidget("HandyMenu", new THandyMenuWidget(this));
    TWidgetManager::getInstance()->getWidget("HandyMenu")->setVisible(false);
    TWidgetManager::getInstance()->addWidget("POIInfo", new TPOIInfoWidget(this));
    TWidgetManager::getInstance()->getWidget("POIInfo")->setVisible(false);

    connect(gps, SIGNAL(positionUpdate(GPSdata)), TWidgetManager::getInstance()->getWidget("SpeedMeter"), SLOT(updateSpeed(GPSdata)));
    connect(gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(positionUpdated(GPSdata)));
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

    ui->widget->resize(size.width(),size.height());

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
    bool val=ui->widget->getTracking();
    if(val)
        ui->widget->setTracking(false);
    else
        ui->widget->setTracking(true);

    ui->trackingButton->setText(ui->widget->getTracking()?"Off":"On");

}

void NavigationWindow::on_routeButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    routeWin->setVisible(true);
    routeWin->raise();
    connect(routeWin, SIGNAL(closed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_optionsButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    optionsWin->setVisible(true);
    optionsWin->raise();
    connect(optionsWin, SIGNAL(closed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_gpsButton_clicked() {
    ui->menuPanel->setVisible(false);
    ui->menuButton->setVisible(false);
    ui->trackingButton->setVisible(false);
    ui->sliderButton->setVisible(false);
    TWidgetManager::getInstance()->hideAllWidgets();
    gpsInfoWin->setVisible(true);
    gpsInfoWin->raise();
    connect(gpsInfoWin, SIGNAL(closed()), this, SLOT(menuClosedSlot()));
}

void NavigationWindow::on_exitButton_clicked() {
    setVisible(false);
    this->close();
}

void NavigationWindow::menuClosedSlot() {
    ui->menuButton->setVisible(true);
    ui->trackingButton->setVisible(true);
    ui->sliderButton->setVisible(true);
    ui->menuPanel->setVisible(false);
    TWidgetManager::getInstance()->showAllWidgets();
}

void NavigationWindow::on_sliderButton_clicked() {
    TWidgetManager::getInstance()->changeMode();
    ui->sliderButton->setIcon(TWidgetManager::getInstance()->getMode() == 1 ?
                                  QIcon("..//Pilocik//images//icons//64x64//lock_open_ico.gif") :
                                  QIcon("..//Pilocik//images//icons//64x64//lock_closed_ico.gif") );
}

void NavigationWindow::positionUpdated(GPSdata gps_data){
    ui->widget->setMyCoordinates(gps_data.lon,gps_data.lat,gps_data.angle);
}

void NavigationWindow::retranslate()
{
    ui->retranslateUi(this);
}

void NavigationWindow::setRoute(QList<osmscout::Routing::Step> route)
{
    mapRenderer->setRoute(route);
    mapRenderer->setRouting(true);
    mapRenderer->setMyCoordinates(route.at(0).lon, route.at(0).lat, 45);
}

void NavigationWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QPointF NavigationWindow::getCoordinates()
{
    return ui->widget->getCoordinates();
}
