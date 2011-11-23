#ifndef MAPRENDERWIDGET_H
#define MAPRENDERWIDGET_H

#include "gpsreceiver.h"
#include <QWidget>
#include <QList>
#include <QPainter>
#include <QPointF>
#include <QPixmap>
#include <QRect>
#include <QString>
#include <QThread>
#include <QVector>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Database.h>
#include <osmscout/StyleConfig.h>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Routing.h>
#include <osmscout/Searching.h>

namespace Ui {
    class MapRenderWidget;
}


class MapPixmapRenderer: public QThread{
    Q_OBJECT
public:
    MapPixmapRenderer(QObject *parent = 0);
    void getPixmap();
    void run();
    /**
      @brief Init
      @param database
      @param projection
      @param style
      @param magnification ratio
      */
    void init(osmscout::Database *database,osmscout::MercatorProjection  *projection,osmscout::StyleConfig*style,float d);
    bool isWorking();
private:
    float d;
    bool started;
    osmscout::Database *database;
    osmscout::MapPainterQt        *mapPainter;
    osmscout::MercatorProjection  *projection;
    osmscout::StyleConfig  *styleConfig;
signals:
    void pixmapRendered(QImage pixmap,osmscout::MercatorProjection projection);


};


/**
  @brief The widget with rendered map.
  */

class MapRenderWidget : public QWidget
{
    Q_OBJECT

    enum PenStyle {
        NORMAL_LINE,
        ROUTE_LINE,
        ROUTE_EDGE_LINE,
    };

public:
    MapRenderWidget(QWidget *parent=0,int width=0,int height=0);
    ~MapRenderWidget();
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    /**
      @brief Settings coordinates.
      */
    void setCoordinates(double lonPar, double latPar);

    /**
      @brief Settings coordinates of actual position.
      */
    void setMyCoordinates(double lonPar, double latPar,double angle);

    /**
      @brief Gettings coordinates.
      @return Actual coordinates.
      */
    QPointF getCoordinates();

    /**
      @brief Settings route.
      @param Actual route.
      */
    void setRoute(QVector<osmscout::Routing::RouteNode> route);

    /**
      @brief Gettings actual route.
      @return Actual route.
      */
    QVector<osmscout::Routing::RouteNode> getRoute();

    /**
      @brief Setting zoom value.
      @param zoom value.
      */
    void setZoom(int value);

    /**
      @brief Getting zoom value.
      @return zoom value.
      */
    int getZoom();
    /**
         @brief Memorizing start zoom value, when zoom level is changed.
         @param value zoom value
     */
    void setStartZoom(int value);

    /**
      @brief Setting finish zoom value and forcing repaint.
      @param value zoom value
      */
    void setFinishZoom(int value);

    /**
         @brief Enable or disable position tracking (auto map moving when setMyCoordinates changed)
         @param value If false tracking is disabled, when true enabled.
      */
    void setTracking(bool tracking);
    /**
      @brief Gets actual tracking value.
      @return Actual tracking value.
      */
    bool getTracking();

    /**
      @brief Enable or disable rendering route.
      @param Actual route rendering value.
      */
    void setRouting(bool routing);

    /**
      @brief Gets actual route rendering value.
      @return Actual route rendering value.
      */
    bool getRouting();

    enum HintType {
        NoHint,
        NormalHint,
        LeaveRouteHint,
        FinishRouteHint
    };

    /**
      @brief Updates hint with specified hint type.
      @param hintType Hint type.
      */
    void updateHint(HintType hintType = NoHint);

private:
    bool tracking;
    bool routing;
    bool manualSimulation;
    bool movingPosition;
    double myLon,myLat,myAngle;
    double nodeR, routeW;   // size of node and route
    osmscout::Searching::Intersection nextIntersection;
    int getNextCrossIndex();

    bool mouseDown;
    //Dodatkowy rozmiar w cache
    double lat,lon;
    int zoom;
    //Pressed
    double lon1,lat1;

    int cachePixmapSize;
    double delta;
    static osmscout::DatabaseParameter databaseParameter;
    static osmscout::Database *database;
    osmscout::StyleConfig     *styleConfig;
    osmscout::MercatorProjection  projection;
    osmscout::MercatorProjection  projectionRendered;
    osmscout::MercatorProjection  projectionRendered1;

    QVector<osmscout::Routing::RouteNode> route;
    int lastNodeIndex;

    MapPixmapRenderer *rendererThread;
    osmscout::Searching *searching;
    QImage image;
    void testPixmap(bool force=false);
    void DrawPositionMarker(const osmscout::Projection& projection,QPainter *painter);
    void DrawRoute(const osmscout::Projection& projection, QPainter *painter);
    QPen setPenStyle(PenStyle penStyle = NORMAL_LINE);
    void setRouteSizes(int zoom);

public slots:

    void newPixmapRendered(QImage image,osmscout::MercatorProjection projection);
    void leaveRoute(double actLon, double actLat, double destLon, double destLat);

};



#endif // MAPRENDERWIDGET_H
