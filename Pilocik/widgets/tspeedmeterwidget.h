#ifndef TSPEEDMETERWIDGET_H
#define TSPEEDMETERWIDGET_H

#include "gpsreceiver.h"
#include "tmovableframe.h"

namespace Ui {
    class TSpeedMeterWidget;
}

class TSpeedMeterWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TSpeedMeterWidget(QWidget *parent = 0);
    ~TSpeedMeterWidget();

private:
    Ui::TSpeedMeterWidget *ui;

public slots:
    void updateSpeed(GPSdata gps_data);
};

#endif // TSPEEDMETERWIDGET_H
