#include "tpoiinfowidget.h"
#include "ui_tpoiinfowidget.h"

TPOIInfoWidget::TPOIInfoWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TPOIInfoWidget)
{
    ui->setupUi(this);
}

TPOIInfoWidget::~TPOIInfoWidget()
{
    delete ui;
}

void TPOIInfoWidget::showEvent(QShowEvent * e)
{
    if(!isPOI) {
        setVisible(false);
    }
}

void TPOIInfoWidget::showInfo(PiLibocik::Poi poi)
{
    ui->poiNameLabel->setText(poi.getName());
    ui->poiLon->setText(QString::number(poi.getLon()));
    ui->poiLat->setText(QString::number(poi.getLat()));

    typedef QPair<QString,QString> tag;
    foreach(tag t, poi.getTags())
    {
        QLabel *l = new QLabel(" " + t.first + " : " + t.second, ui->poiDetails);
        ui->poiDetails->layout()->addWidget(l);
    }

    isPOI = true;
    setVisible(true);
}

void TPOIInfoWidget::on_okButton_clicked()
{
    isPOI = false;

    setVisible(false);
}
