#ifndef GPSOPTIONSWINDOW_H
#define GPSOPTIONSWINDOW_H

#include "qfullscreenframe.h"

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

private:
    Ui::GPSOptionsWindow *ui;
};

#endif // GPSOPTIONSWINDOW_H
