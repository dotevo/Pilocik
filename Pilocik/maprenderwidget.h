#ifndef MAPRENDERWIDGET_H
#define MAPRENDERWIDGET_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QPoint>
#include <QPixmap>

namespace Ui {
    class MapRenderWidget;
}

class MapRenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapRenderWidget(QWidget *parent = 0);
    ~MapRenderWidget();
    int DrawMap();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:

    std::string   map;
    std::string   style;
    std::string   output;
    size_t        width,height;
    double        lon,lat,zoom;

    QPixmap pixmap;
    // map operations
    QPoint lastCoord;
    bool moving;
    bool scaling;
    bool nothing;

    QPoint startPoint;  // mouse is pressed
    QPoint finishPoint; // mouse is released
    QPoint lastPoint;   // last position of mouse
    QPoint translatePoint;  // translation point

    void init();
};

#endif // MAPRENDERWIDGET_H
