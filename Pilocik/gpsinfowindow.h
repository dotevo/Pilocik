#ifndef GPSINFOWINDOW_H
#define GPSINFOWINDOW_H

#include "qfullscreenframe.h"
#include "gpsreceiver.h"

namespace Ui {
    class GPSInfoWindow;
}

class GPSInfoWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit GPSInfoWindow(NavigationWindow *parent = 0);
    ~GPSInfoWindow();

private slots:
    void on_gpsInfoBackButton_clicked();
    void on_startGPSButton_clicked();
    void on_stopGPSButton_clicked();
    void infoUpdate(GPSdata gps_data);
    void statusUpdate(QString status);
    void clearInfo();

private:
    QString status;
    GPSreceiver* gps;
    Ui::GPSInfoWindow *ui;
};

#endif // GPSINFOWINDOW_H
