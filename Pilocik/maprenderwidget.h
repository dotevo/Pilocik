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
    typedef QWidget widget;

    explicit MapRenderWidget(QWidget *parent = 0);
    ~MapRenderWidget();
    /**
      @brief Drawing map.
      @param rect rectangle using during rendering transformated map.
      */
    int DrawMap(QRect rect);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void repaint();

    /**
      @brief Setting zoom value.
      @param zoom value.
      */
    void setZoom(int value);

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

private:
    GPSreceiver* gps;
    QString   map;
    QString   style;
    QString   output;
    size_t    width,height;
    double    lon,lat,angle,zoom;

    QPixmap pixmap;

    bool noPaint;
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

    /**
      @brief Initializing variables depending of the operating system.
      */
    void init();

private slots:
    void positionUpdate(GPSdata gps_data);
};

#endif // MAPRENDERWIDGET_H
