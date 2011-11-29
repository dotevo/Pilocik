#include "thandymenuwidget.h"
#include "ui_thandymenuwidget.h"
#include "../navigationwindow.h"
#include "../routewindow.h"
#include "../maprenderwidget.h"

THandyMenuWidget::THandyMenuWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::THandyMenuWidget)
{
    ui->setupUi(this);

    this->setFixedHeight(90);

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
    this->setFixedHeight(120);
    poiClicked = clicked;
}

void THandyMenuWidget::setPOIInfoWindow(InfoWindow * info)
{
    infoWindow = info;
}

void THandyMenuWidget::setPosition(double lon, double lat)
{
    position.setX(lon);
    position.setY(lat);
}

void THandyMenuWidget::showMenu(int x, int y)
{
    shown = true;
    clicked.setX(x);
    clicked.setY(y);
    move(clicked);
    setVisible(true);
}

void THandyMenuWidget::on_poInfoButton_clicked()
{
    infoWindow->setVisible(true);
    poiClicked = false;
    shown = false;
    this->setFixedHeight(90);
    setVisible(false);
}

void THandyMenuWidget::on_navFromButton_clicked()
{
    double lon, lat;
    NavigationWindow::main->routeWin->targetSet(position.x(), position.y(), "Picked in map view");
    poiClicked = false;
    shown = false;
    this->setFixedHeight(90);
    setVisible(false);
}

void THandyMenuWidget::on_navToButton_clicked()
{
    double lon, lat;
    NavigationWindow::main->routeWin->targetSet(position.x(), position.y(), "Picked in map view");
    poiClicked = false;
    shown = false;
    this->setFixedHeight(90);
    setVisible(false);
}

void THandyMenuWidget::on_cancelButton_clicked()
{
    poiClicked = false;
    shown = false;
    this->setFixedHeight(90);
    setVisible(false);
}
