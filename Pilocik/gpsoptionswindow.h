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

    /**
      @brief Sets focus at startup to something other than qlineedit object.
      */
    void setStartFocus();

private slots:
    void on_okButton_clicked();
    void on_startSimButton_clicked();
    void on_stopSimButton_clicked();
    void simStatusUpdate(QString string);

signals:
    void closed();

private:
    GPSreceiver* gps;
    Ui::GPSOptionsWindow *ui;

    void changeEvent(QEvent *);
};

#endif // GPSOPTIONSWINDOW_H
