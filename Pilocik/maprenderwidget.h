#ifndef MAPRENDERWIDGET_H
#define MAPRENDERWIDGET_H

#include "gpsreceiver.h"
#include <QWidget>
#include <QList>
#include <QPainter>
#include <QPoint>
#include <QPixmap>
#include <QRect>
#include <QString>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Database.h>
#include <osmscout/StyleConfig.h>
#include <osmscout/MapPainterQt.h>

namespace Ui {
    class MapRenderWidget;
}
/**
  @brief The widget with rendered map.
  */
class MapRenderWidget : public QWidget
{
    Q_OBJECT

public:
    /**
      @brief Creating map render widget.
      @param parent Parent widget.
      @param W Width of rendering area.
      @param H Height of rendering area.
      */
    explicit MapRenderWidget(QWidget *parent = 0, int W = 0, int H = 0);
    typedef QWidget widget;

    ~MapRenderWidget();

    /**
      @brief Initializing variables depending of the operating system.
      */
    void init(int W = 0, int H = 0);

    /**
      @brief Drawing map.
      @param rect rectangle using during rendering transformated map.
      */
    int DrawMap(QRect rect);

    /**
     * @brief
     * @param
     */
    void DrawPartitions();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void repaint();

    /**
      @brief Settings coordinates.
      */
    void setCoordinates(double lonPar, double latPar);

    /**
      @brief Gettings coordinates.
      @return Actual coordinates.
      */
    QPoint getCoordinates();

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
      @brief Forcing repaint.
      */
    void forceRepaint();

    /**
      @brief Enable or disable position tracking (auto map moving when position changes)
      @param value If false tracking is disabled, when true enabled.
      */
    void setTracking(bool tracking);
    /**
      @brief Gets actual tracking value.
      @return Actual tracking value.
      */
    bool getTracking();
    /**
      @brief Sets new size for rendering area.
      @param size New size value.
      */
    void setSize(QSize size);

private:
    osmscout::DatabaseParameter databaseParameter;
    osmscout::Database          *database;
    osmscout::StyleConfig       *styleConfig;

    osmscout::MapPainterQt        *mapPainter;

    GPSreceiver* gps;
    QString   map;
    QString   style;
    QString   output;
    size_t    width,height;
    double    lon,lat,markerLon,markerLat,angle,zoom;
    bool      tracking, debugPartitions;

    QPixmap pixmap;
    osmscout::MapPainterQt partitionMapPainter;

    bool noPaint;
    bool isDrawn;
    bool gpsActive;

    bool moving;
    QPoint startPoint;  // mouse is pressed
    QPoint finishPoint; // mouse is released
    QPoint lastPoint;   // last position of mouse
    QPoint translatePoint;  // translation point

    bool scaling;
    double scalingLevel;
    int startZoom;  // mouse is pressed, 'catching' slider
    int finishZoom; // mouse is released, 'releasing' slider

private slots:
    void positionUpdate(GPSdata gps_data);
};

#endif // MAPRENDERWIDGET_H
