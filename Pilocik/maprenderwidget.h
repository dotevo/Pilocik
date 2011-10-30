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
#include <QThread>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Database.h>
#include <osmscout/StyleConfig.h>
#include <osmscout/MapPainterQt.h>

namespace Ui {
    class MapRenderWidget;
}


class MapPixmapRenderer: public QThread{
    Q_OBJECT
public:
    MapPixmapRenderer();
    void getPixmap();
    void run();
    void init(osmscout::Database *database,osmscout::MercatorProjection  *projection,osmscout::StyleConfig*style);
private:
    bool started;
    osmscout::Database *database;
    osmscout::MapPainterQt        *mapPainter;
    osmscout::MercatorProjection  *projection;
    osmscout::StyleConfig  *styleConfig;
signals:
    void pixmapRendered(QPixmap pixmap,osmscout::MercatorProjection projection);


};


/**
  @brief The widget with rendered map.
  */

class MapRenderWidget : public QWidget
{
    Q_OBJECT

public:
    MapRenderWidget(QWidget *parent=0,int width=0,int height=0);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);


private:
    bool mouseDown;
    //Dodatkowy rozmiar w cache
    int lat,lon,zoom;

    int cachePixmapSize;
    static osmscout::DatabaseParameter databaseParameter;
    static osmscout::Database *database;
    osmscout::StyleConfig     *styleConfig;
    osmscout::MercatorProjection  projection;
    osmscout::MercatorProjection  projectionRendered;
    osmscout::MercatorProjection  projectionRendered1;

    MapPixmapRenderer *rendererThread;
    QPixmap pixmap;
    void testPixmap();

public slots:
    void newPixmapRendered(QPixmap pixmap,osmscout::MercatorProjection projection);
};



#endif // MAPRENDERWIDGET_H
