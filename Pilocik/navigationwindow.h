#ifndef NAVIGATIONWINDOW_H
#define NAVIGATIONWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <gpsreceiver.h>
#include "maprenderwidget.h"
#include "osmscout/Routing.h"

namespace Ui {
    class NavigationWindow;
}

class RouteWindow;
class OptionsWindow;
class GPSInfoWindow;

class NavigationWindow : public QMainWindow
{
    Q_OBJECT
public:
    GPSreceiver* gps;
    explicit NavigationWindow(QWidget *parent = 0);
    ~NavigationWindow();
    void    resizeEvent ( QResizeEvent * event );
    static NavigationWindow *main;
    MapRenderWidget *mapRenderer;

    void retranslate();
    void setRoute(QVector<osmscout::Routing::RouteNode> route);
    QPointF getCoordinates();

private slots:
    void on_menuButton_clicked();
    void on_trackingButton_clicked();
    void on_routeButton_clicked();
    void on_optionsButton_clicked();
    void on_gpsButton_clicked();
    void on_exitButton_clicked();
    /**
     * @brief Shows all widgets after exiting from options.
     */
    void menuClosedSlot();
    void on_sliderButton_clicked();
    void positionUpdated(GPSdata gps_data);
signals:
    void sizeChanged(QWidget *widget);

private:
    Ui::NavigationWindow *ui;
    void addWidgets();
    void addFrames();

    OptionsWindow *optionsWin;
    GPSInfoWindow *gpsInfoWin;

    void changeEvent(QEvent *);

public:
    RouteWindow *routeWin;
};

#endif // NAVIGATIONWINDOW_H
