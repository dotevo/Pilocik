#ifndef GPSOPTIONSWINDOW_H
#define GPSOPTIONSWINDOW_H

#include "qfullscreenframe.h"
#include "gpsreceiver.h"

namespace Ui {
    class GPSOptionsWindow;
}

class NavigationWindow;

class GPSOptionsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit GPSOptionsWindow(NavigationWindow *parent);
    ~GPSOptionsWindow();

private slots:
    void on_okButton_clicked();
    void on_startSimButton_clicked();
    void on_stopSimButton_clicked();

private:
    GPSreceiver* gps;
    Ui::GPSOptionsWindow *ui;
};

#endif // GPSOPTIONSWINDOW_H
