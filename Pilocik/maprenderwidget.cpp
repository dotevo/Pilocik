#include "maprenderwidget.h"
#include "navigationwindow.h"

#include <iostream>
#include <iomanip>

#include <QFileInfo>
#include <QDir>

#include <QMouseEvent>
#include <QLineF>

#include <QtGui/QPixmap>
#include <QtGui/QApplication>

#include <osmscout/Database.h>
#include <osmscout/StyleConfigLoader.h>
#include <osmscout/Util.h>
#include <osmscout/MapPainterQt.h>

MapRenderWidget::MapRenderWidget(QWidget* parent) :
    QWidget(parent)
{
    init();
}

MapRenderWidget::~MapRenderWidget() { }

void MapRenderWidget::init()
{
    moving = false;
    scaling = false;
    nothing = true;

    map = "";
    style = "";

#ifdef Q_OS_UNIX
     map = "/home/bartek/osmscout-map/3poland/";
    style = "/home/bartek/QtProjects/OSMNavi/styles/standard.oss.xml";
#endif

#ifdef Q_OS_WIN
    map = "c:/map/";
    style = "c:/map/standard.oss.xml";
#endif

    translatePoint = QPoint(0, 0);
    lastPoint = QPoint(0, 0);

    //width = 640;
    //height = 480;
    width = 673;
    height = 378;
    lat = 51.1;
    lon = 17.03;
    zoom = 2*2*2*2*1024;

    pixmap = QPixmap(width, height);
    pixmap.fill(QColor(200, 200, 200));

}

void MapRenderWidget::paintEvent(QPaintEvent *)
{
    if (!nothing)
        DrawMap();
}


void MapRenderWidget::mousePressEvent(QMouseEvent *e)
{
    nothing = false;
    moving = true;
    startPoint = e->globalPos();
    lastPoint = e->globalPos();
}

void MapRenderWidget::mouseReleaseEvent(QMouseEvent *e)
{
    moving = false;

    finishPoint = e->globalPos();
    QLineF line(startPoint, finishPoint);

    double dx = line.dx();
    double dy = line.dy();

    lon -= dx/(2*zoom);
    lat += dy/(2*zoom);

    translatePoint = QPoint(0, 0);
    startPoint = QPoint(0, 0);

    repaint();
    nothing = true;

}

void MapRenderWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (moving)
    {
        translatePoint = e->globalPos() - startPoint;

        repaint();
    }
}

int MapRenderWidget::DrawMap()
{
    // std::cerr << "DrawMapQt <map directory> <style-file> <width> <height> <lon> <lat> <zoom> <output>" << std::endl;
//    std::cerr << "Default values!";

    if (moving)
    {
        int x = translatePoint.x();
        int y = translatePoint.y();

        QPainter *windowPainter = new QPainter(this);
        windowPainter->drawPixmap(x, y, pixmap);
    }

    else if (scaling)
    {

    }

    else
    {
        osmscout::DatabaseParameter databaseParameter;
        osmscout::Database          database(databaseParameter);

        if (!database.Open(map.c_str())) {
            std::cerr << "Cannot open database" << std::endl;
            return 1;
        }

        osmscout::StyleConfig styleConfig(database.GetTypeConfig());

        if (!osmscout::LoadStyleConfig(style.c_str(),styleConfig)) {
            std::cerr << "Cannot open style" << std::endl;
        }

        QPainter* painter = new QPainter(&pixmap);

        if (painter!=NULL) {
            osmscout::MercatorProjection  projection;
            osmscout::MapParameter        drawParameter;
            osmscout::AreaSearchParameter searchParameter;
            osmscout::MapData             data;
            osmscout::MapPainterQt        mapPainter;

            projection.Set(lon,
                           lat,
                           zoom,
                           width,
                           height);


            database.GetObjects(styleConfig,
                                projection.GetLonMin(),
                                projection.GetLatMin(),
                                projection.GetLonMax(),
                                projection.GetLatMax(),
                                projection.GetMagnification(),
                                searchParameter,
                                data.nodes,
                                data.ways,
                                data.areas,
                                data.relationWays,
                                data.relationAreas);

            if (mapPainter.DrawMap(styleConfig,
                                   projection,
                                   drawParameter,
                                   data,
                                   painter)) {
 //               std::cerr << "Drawing!" << std::endl;
 //               std::cerr << "Zoom: " << zoom << std::endl;

            }

            delete painter;

            QPainter *windowPainter = new QPainter(this);
            windowPainter->drawPixmap(0, 0, pixmap);
        }
        else {
            std::cout << "Cannot create QPainter" << std::endl;
        }
    }
}
