#include "thandymenuwidget.h"
#include "ui_thandymenuwidget.h"
#include "../navigationwindow.h"
#include "../routewindow.h"
#include "../maprenderwidget.h"
#include "../twidgetmanager.h"
#include "tpoiinfowidget.h"

THandyMenuWidget::THandyMenuWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::THandyMenuWidget)
{
    ui->setupUi(this);

    this->setFixedHeight(120);

    poiClicked = false;
    shown = false;
}

THandyMenuWidget::~THandyMenuWidget()
{
    delete ui;
}

void THandyMenuWidget::showEvent(QShowEvent * e)
{
    if(!shown) {
        setVisible(false);
    } else if(poiClicked) {
        ui->poInfoButton->setVisible(true);
    } else {
        ui->poInfoButton->setVisible(false);
    }
}

void THandyMenuWidget::setPOIClicked(bool clicked)
{
    this->setFixedHeight(150);
    poiClicked = clicked;
}

void THandyMenuWidget::setPOI(PiLibocik::Poi poi)
{
    this->poi = poi;
}

void THandyMenuWidget::setPosition(double lon, double lat)
{
    position.setLon(lon);
    position.setLat(lat);
}

void THandyMenuWidget::showMenu(int x, int y)
{
    QSize windowSize = NavigationWindow::main->size();
    shown = true;
    clicked.setX(x);
    clicked.setY(y);
    if(clicked.x() > windowSize.width()-this->width()) {
        clicked.setX(clicked.x()-this->width());
    }
    if(clicked.y() > windowSize.height()-this->height()) {
        clicked.setY(clicked.y()-this->height());
    }
    move(clicked);
    setVisible(true);
}

void THandyMenuWidget::on_poInfoButton_clicked()
{
    ((TPOIInfoWidget *) TWidgetManager::getInstance()->getWidget("POIInfo"))->showInfo(poi);
    reset();
}

void THandyMenuWidget::on_navFromButton_clicked()
{ 
    NavigationWindow::main->routeWin->startSet(position.getLon(), position.getLat(), (poiClicked) ? "POI: " + poi.getName() : "Picked in map view");
    reset();
}

void THandyMenuWidget::on_navToButton_clicked()
{
    NavigationWindow::main->routeWin->targetSet(position.getLon(), position.getLat(), (poiClicked) ? "POI: " + poi.getName() : "Picked in map view");
    reset();
}

void THandyMenuWidget::on_navThroughButton_clicked()
{
    NavigationWindow::main->routeWin->addStop(position.getLon(), position.getLat(), (poiClicked) ? "POI: " + poi.getName() : "Picked in map view");
    reset();
}

void THandyMenuWidget::on_cancelButton_clicked()
{
    reset();
}

void THandyMenuWidget::reset()
{
    position = PiLibocik::Position();
    poi = PiLibocik::Poi();
    poiClicked = false;
    shown = false;
    this->setFixedHeight(120);
    setVisible(false);
}
