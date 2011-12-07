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
    /**
      @brief Resize evenet overload to keep buttons in correct place.
      @param event Resize event.
      */
    void    resizeEvent ( QResizeEvent * event );
    static NavigationWindow *main;
    MapRenderWidget *mapRenderer;

    /**
      @brief Retranslates application after language change.
      */
    void retranslate();
    /**
      @brief Sets route that will be rendered.
      @param route Route as list of routing steps.
      */
    void setRoute(QList<osmscout::Routing::Step> route);

    /**
      @brief Returns map coordinates.
      @return Coordinates of map central point.
      */
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
    void startedSim();
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
