#include "infowindow.h"
#include "ui_infowindow.h"

#include <QDebug>

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

void InfoWindow::setMapRenderAreaSize(QSize size)
{
    ui->mapWidget->setSize(size);
}

void InfoWindow::setMapRender()
{
    ui->mapWidget = new MapRenderWidget(this, getSize().width(), getSize().height());
    qDebug() << getSize().width();
    setMapRenderAreaSize(QSize(500, 500));

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
