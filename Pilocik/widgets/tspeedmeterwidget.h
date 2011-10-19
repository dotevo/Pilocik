#ifndef TSPEEDMETERWIDGET_H
#define TSPEEDMETERWIDGET_H

#include "../twidget.h"
#include "gpsreceiver.h"

namespace Ui {
    class TSpeedMeterWidget;
}

class TSpeedMeterWidget : public TWidget
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
