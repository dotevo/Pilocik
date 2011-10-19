#include "maprenderwidget.h"
#include "navigationwindow.h"

#include "qmath.h"
#include "gpsreceiver.h"

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
    noPaint = true;
    gpsActive = false;

    map = "";
    style = "";

#ifdef Q_OS_UNIX
     map = "/home/bartek/osmscout-map/3poland/";
    style = "/home/bartek/QtProjects/OSMNavi/styles/standard.oss2.xml";
#endif

#ifdef Q_OS_WIN
    map = "c:/map/";
    style = "c:/map/standard.oss.xml";
#endif

#ifdef WINCE
    map = "/ResidentFlash/ZPI/map";
    style = "/ResidentFlash/ZPI/standard.oss.xml";
#endif

    translatePoint = QPoint(0, 0);
    lastPoint = QPoint(0, 0);

    //width = 640;
    //height = 480;
    width = 673;
    height = 378;
    angle = 0;
    lat = 51.1;
    lon = 17.03;
    zoom = 2*2*2*2*1024;

    pixmap = QPixmap(width, height);
    pixmap.fill(QColor(200, 200, 200));


    NavigationWindow* navi = (NavigationWindow*)(this->parent()->parent());
    gps = navi->gps;
    connect(gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(positionUpdate(GPSdata)));
}

void MapRenderWidget::forceRepaint()
{
    noPaint = false;
    repaint();
    noPaint = true;
}

void MapRenderWidget::setZoom(int value)
{
    noPaint = false;

    scaling = true;

    zoom = value;
    scalingLevel = zoom / startZoom;

    repaint();

    scaling = false;
    noPaint = true;
}

void MapRenderWidget::setStartZoom(int value)
{
    startZoom = value;
}

void MapRenderWidget::setFinishZoom(int value)
{
    finishZoom = value;

    scaling = false;

    forceRepaint();
}

void MapRenderWidget::paintEvent(QPaintEvent *e)
{
    if (!noPaint)
        DrawMap(e->rect());
}

void MapRenderWidget::mousePressEvent(QMouseEvent *e)
{
    noPaint = false;
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
    noPaint = true;
}

void MapRenderWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (moving)
    {
        translatePoint = e->globalPos() - startPoint;

        repaint();
    }
}

int MapRenderWidget::DrawMap(QRect rect)
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
        QPainter *windowPainter = new QPainter(this);

        double distX = width/2 * (double)(1 - scalingLevel);
        double distY = height/2 * (double)(1 - scalingLevel);

        windowPainter->translate(distX, distY);
        windowPainter->scale(scalingLevel, scalingLevel);

        QRect expose = windowPainter->matrix().inverted().mapRect(rect).adjusted(-1, -1, 1, 1);

        windowPainter->drawPixmap(expose, pixmap, expose);
        //windowPainter->drawPixmap(0, 0, pixmap);
    }

    else
    {
        osmscout::DatabaseParameter databaseParameter;
        osmscout::Database          database(databaseParameter);

		if (!database.Open((const char*)map.toAscii())) {
            std::cerr << "Cannot open database" << std::endl;
            return 1;
        }

        osmscout::StyleConfig styleConfig(database.GetTypeConfig());

        if (!osmscout::LoadStyleConfig((const char*)style.toAscii(),styleConfig)) {
            std::cerr << "Cannot open style" << std::endl;
        }

        QPainter* painter = new QPainter(&pixmap);

        if (painter!=NULL) {
            osmscout::MercatorProjection  projection;
            osmscout::MapParameter        drawParameter;
            osmscout::AreaSearchParameter searchParameter;
            osmscout::MapData             data;
            osmscout::MapPainterQt        mapPainter;

            drawParameter.SetOptimizeAreaNodes(true);
            drawParameter.SetOptimizeWayNodes(true);

            projection.Set(lon,
                           lat,
                           angle,
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
                                   painter,
                                   gpsActive)) {
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

void MapRenderWidget::positionUpdate(GPSdata gps_data)
{
    lat = gps_data.lat;
    lon = gps_data.lon;
    angle = gps_data.angle;
    gpsActive = (lat!=0 && lon!= 0);
    if(gpsActive)
        forceRepaint();
}
