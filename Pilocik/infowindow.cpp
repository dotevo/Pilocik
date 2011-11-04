#include "infowindow.h"
#include "ui_infowindow.h"

#include <QDebug>

InfoWindow::InfoWindow(QFrame *parent) :
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

void InfoWindow::setZoom(const int zoom)
{
    //ui->mapWidget->setFinishZoom(zoom);
    ui->mapWidget->setZoom(zoom);
}

void InfoWindow::setMapRender()
{
    ui->mapWidget = new MapRenderWidget(this, getSize().width(), getSize().height());
    ui->mapWidget->forceRepaint();
}

QSize InfoWindow::getSize()
{
    return QSize(ui->mapWidget->geometry().width(), ui->mapWidget->geometry().height());
}

void InfoWindow::forceMapWidgetRepaint()
{
    ui->mapWidget->forceRepaint();
}

void InfoWindow::on_backButton_clicked()
{
    this->close();
}
