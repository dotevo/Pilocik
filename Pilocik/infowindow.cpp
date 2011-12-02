#include "infowindow.h"
#include "ui_infowindow.h"
#include <pilibocik/poi.h>

#include <QDebug>

InfoWindow::InfoWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
    ui->mapWidget->setCacheSettings(1, 0.0);
//    ui->mapWidget->setSize(getSize());
}

InfoWindow::~InfoWindow()
{
    delete ui->mapWidget;
    delete ui;
}

void InfoWindow::setPoiType(int type)
{
    QMap<int, PiLibocik::PoiDisplay> poiDisplay = Settings::getInstance()->getPoiDisplaySettings();
    setZoom(poiDisplay[type].getZoom()*4);
}

void InfoWindow::setName(const QString name)
{
    this->name = name;
    ui->nameText->setText(name);
}

QString InfoWindow::getPOIName(){
    return name;
}

void InfoWindow::setCoordinates(const double lon, const double lat)
{
    this->lon = lon;
    this->lat = lat;

    ui->lonText->setText(" Lon: "+QString::number(lon));
    ui->latText->setText(" Lat: "+QString::number(lat));

    ui->mapWidget->setCoordinates(lon, lat);
    ui->mapWidget->insertArrow(lon,lat);
    ui->mapWidget->repaint();
}

void InfoWindow::setZoom(const int zoom)
{
    ui->mapWidget->setZoom(zoom);
}

void InfoWindow::setMapRender()
{
    ui->mapWidget = new MapRenderWidget(this, getSize().width(), getSize().height());
}

QSize InfoWindow::getSize()
{
    return QSize(ui->mapWidget->geometry().width(), ui->mapWidget->geometry().height());
}

void InfoWindow::forceMapWidgetRepaint()
{
    //ui->mapWidget->forceRepaint();
}

void InfoWindow::setDetails(QList < QPair <QString, QString> > tags)
{
    typedef QPair<QString,QString> tag;
    foreach(tag t, tags)
    {
        QLabel *l = new QLabel(" "+t.first + " : " + t.second, ui->infoWidget);
        ui->infoData->layout()->addWidget(l);
    }
}

void InfoWindow::on_backButton_clicked()
{
    this->close();
}

void InfoWindow::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
