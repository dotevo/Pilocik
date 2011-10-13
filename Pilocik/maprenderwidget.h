#ifndef MAPRENDERWIDGET_H
#define MAPRENDERWIDGET_H

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

class MapRenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapRenderWidget(QWidget *parent = 0);
    ~MapRenderWidget();
    int DrawMap(QRect rect);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void setZoom(int value);
    void setStartZoom(int value);
    void setFinishZoom(int value);

    void forceRepaint();

private:

    QString   map;
    QString   style;
    QString   output;
    size_t        width,height;
    double        lon,lat,zoom;

    QPixmap pixmap;

    QPoint lastCoord;

    bool noPaint;

    bool moving;
    QPoint startPoint;  // mouse is pressed
    QPoint finishPoint; // mouse is released
    QPoint lastPoint;   // last position of mouse
    QPoint translatePoint;  // translation point

    bool scaling;
    double scalingLevel;
    int startZoom;  // mouse is pressed, 'catching' slider
    int finishZoom; // mouse is released, 'releasing' slider

    void init();
};

#endif // MAPRENDERWIDGET_H
