#include "infowindow.h"
#include "ui_infowindow.h"

InfoWindow::InfoWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

void InfoWindow::setName(const QString name)
{
    this->name = name;

    ui->nameText->setText(name);
}

void InfoWindow::setCoordinates(const double lat, const double lon)
{
    this->lon = lon;
    this->lat = lat;

    ui->lonText->setText(QString::number(lon));
    ui->latText->setText(QString::number(lat));

    ui->mapWidget->setCoordinates(lat, lon);
}

void InfoWindow::setMapRender()
{
    int width = ui->mapWidget->geometry().width();
    int height = ui->mapWidget->geometry().height();

    ui->mapWidget = new MapRenderWidget(width, height, this, lat, lon);
    //ui->mapWidget->setCoordinates(lat, lon);

    ui->mapWidget->forceRepaint();
}

void InfoWindow::on_backButton_clicked()
{
    this->close();
}
