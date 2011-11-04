#ifndef PARTITIONSRENDERWIDGET_H
#define PARTITIONSRENDERWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QPainter>
#include <QPoint>
#include <QTime>
#include <QList>
#include <QMap>
#include <QMouseEvent>
#include <QPixmap>
#include <math.h>


class PartitionsRenderWidget : public QWidget
{
    Q_OBJECT

    struct pNode{
        double lat;
        double lon;
        int cellNo;
    };

    struct pWay{
        int priority;
        QList<int> nodes;
    };

    static const double gradtorad=2*M_PI/360;

public:
    explicit PartitionsRenderWidget(QWidget *parent = 0);
    void init(QString dbPath);
    void calculateNewMapCenter(double lon, double lat, double magnification);
    void renderPartitions();
    void geoToPixel(double lon, double lat, double& x, double& y);
    void pixelToGeo(double x, double y, double& lon, double& lat);

    void paintEvent(QPaintEvent * e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    QSqlDatabase db;
    QMap<int, pNode> pNodes;
    QMap<int, pWay > pWays;
    QList<QColor> pColors;

    //! Map render variables
    double  lat,
            lon,
            boxWidth,
            boxHeight,
            lonMin,
            lonMax,
            latMin,
            latMax,
            hmin,
            hmax,
            vmin,
            vmax,
            hscale,
            vscale,
            magnification;
     int    width,
            height;

     bool   mapDrag,
            showWays,
            showNodes;

     QPoint dragStart,
            dragMove,
            dragEnd;

     QPixmap pixmap;

signals:

public slots:
     void changeZoom(int zoom);
     void showNodesChange(int b);
     void showWaysChange(int b);

};

#endif // PARTITIONSRENDERWIDGET_H
