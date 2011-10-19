#include "tspeedmeterwidget.h"
#include "ui_tspeedmeterwidget.h"

TSpeedMeterWidget::TSpeedMeterWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TSpeedMeterWidget)
{
    ui->setupUi(this);
    move(5, 45);
}

TSpeedMeterWidget::~TSpeedMeterWidget()
{
    delete ui;
}

void TSpeedMeterWidget::updateSpeed(GPSdata gps_data)
{
    ui->speedValue->setText(QString::number(gps_data.speed, 'f', 0));
}
