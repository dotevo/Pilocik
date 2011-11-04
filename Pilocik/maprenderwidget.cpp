#include "maprenderwidget.h"
#include "navigationwindow.h"

#include "qmath.h"
#include "gpsreceiver.h"
#include "settings.h"

#include <iostream>
#include <iomanip>

#include <QFileInfo>
#include <QDir>

#include <QDebug>

#include <QMouseEvent>
#include <QLineF>

#include <QtGui/QPixmap>
#include <QtGui/QApplication>

#include <osmscout/Database.h>
#include <osmscout/StyleConfigLoader.h>
#include <osmscout/Util.h>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Node.h>

MapRenderWidget::MapRenderWidget(QWidget *parent, int W, int H)
   : QWidget(parent)
{
    init(W, H);
}

MapRenderWidget::~MapRenderWidget()
{
    Settings::getInstance()->modifyMapSettings(lat, lon, zoom);
}

void MapRenderWidget::setTracking(bool tracking)
{
    this->tracking = tracking;
}

bool MapRenderWidget::getTracking()
{
    return tracking;
}

void MapRenderWidget::init(int W, int H)
{
    map = Settings::getInstance()->getMapPath();
    style = Settings::getInstance()->getMapStylePath();

    if(map.size()==0) {
        std::cerr << "It's not a map, it's a shit!" << std::endl;
    }

    database = new osmscout::Database(databaseParameter);

    if (!database->Open((const char*)map.toAscii())) {
        std::cerr << "Cannot open database" << std::endl;
    }

    styleConfig = new osmscout::StyleConfig(database->GetTypeConfig());

    if (!osmscout::LoadStyleConfig((const char*)style.toAscii(),*(styleConfig))) {
        std::cerr << "Cannot open style" << std::endl;
    }

    mapPainter = new osmscout::MapPainterQt();

    moving = false;
    scaling = false;
    noPaint = false;
    isDrawn = false;
    gpsActive = false;
    tracking = true;

//! Change value to true manually to enable partitions rendering
    debugPartitions = false;

    Settings* settings = Settings::getInstance();
    lat = settings->getLat();
    lon = settings->getLon();
    zoom = settings->getZoom();

    translatePoint = QPoint(0, 0);
    lastPoint = QPoint(0, 0);

    lat = 51.1;
    lon = 17.03;

    zoom = 2*2*2*2*1024;

    angle = 0;

    width = W != 0 ? W : 673;
    height = H != 0 ? H : 378;

    setSize(QSize(width, height));

    pixmap = QPixmap(width, height);
    pixmap.fill(QColor(200, 200, 200));

    NavigationWindow* navi = (NavigationWindow*)(this->parent()->parent());

    gps = navi->gps;
//    connect(gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(positionUpdate(GPSdata)));
}

void MapRenderWidget::setSize(QSize size)
{
    width = size.width();
    height = size.height();
    this->resize(size);
    pixmap = QPixmap(width, height);
    pixmap.fill(QColor(200, 200, 200));
}

void MapRenderWidget::forceRepaint()
{
    noPaint = false;
    repaint();
    noPaint = true;
}

void MapRenderWidget::setCoordinates(double latPar, double lonPar)
{
    lon = lonPar;
    lat = latPar;

    forceRepaint();
}

QPointF MapRenderWidget::getCoordinates()
{
    QPointF point(lon, lat);

    return point;
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

int MapRenderWidget::getZoom()
{
    return zoom;
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

void MapRenderWidget::repaint(){
    widget::repaint();
}

void MapRenderWidget::paintEvent(QPaintEvent *e)
{
    if (isDrawn) {
        if (!noPaint){
            DrawMap(e->rect());

            if(debugPartitions)
                DrawPartitions();
        }
        else{
            int x = translatePoint.x();
            int y = translatePoint.y();
            QPainter *windowPainter = new QPainter(this);
            windowPainter->drawPixmap(x, y, pixmap);
        }
    } else {
        DrawMap(e->rect());
        isDrawn = true;
        noPaint = true;
    }
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
    //std::cerr << "Mouse move";
    if (moving)
    {
        translatePoint = e->globalPos() - startPoint;

        repaint();
    }
}

int MapRenderWidget::DrawMap(QRect rect)
{
    //std::cerr << lon << " | " << lat << std::endl;
    //std::cerr << width << " | " << height << std::endl;
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
        QPainter* painter = new QPainter(&pixmap);

        if (painter!=NULL) {
            osmscout::MercatorProjection  projection;
            osmscout::MapParameter        drawParameter;
            osmscout::AreaSearchParameter searchParameter;
            osmscout::MapData             data;

            drawParameter.SetOptimizeAreaNodes(true);
            drawParameter.SetOptimizeWayNodes(true);

            projection.Set(lon,
                           lat,
                           markerLon,
                           markerLat,
                           angle,
                           zoom,
                           width,
                           height);


            database->GetObjects(*(styleConfig),
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

            mapPainter->DrawMap(*(styleConfig),
                                   projection,
                                   drawParameter,
                                   data,
                                   painter,
                                   gpsActive);

            delete painter;

            QPainter *windowPainter = new QPainter(this);
            windowPainter->drawPixmap(0, 0, pixmap);

        }
        else {
            std::cout << "Cannot create QPainter" << std::endl;
        }
    }
	return 1;
}

void MapRenderWidget::DrawPartitions()
{
    osmscout::MercatorProjection  projection;

    projection.Set(lon,
                   lat,
                   markerLon,
                   markerLat,
                   angle,
                   zoom,
                   width,
                   height);

    QPainter* painter = new QPainter(&pixmap);

    partitionMapPainter.PreparePartitionData("H:\\Users\\Rafael\\Desktop\\partitionOut.txt");
    partitionMapPainter.RenderPartitionResults(projection, painter);

    delete painter;

    QPainter *windowPainter = new QPainter(this);
    windowPainter->drawPixmap(0, 0, pixmap);
}

void MapRenderWidget::positionUpdate(GPSdata gps_data)
{
    markerLat = gps_data.lat;
    markerLon = gps_data.lon;
    if(tracking){
        lat=markerLat;
        lon=markerLon;
    }
    angle = gps_data.angle;
    gpsActive = (lat!=0 && lon!= 0);
    if(gpsActive)
        forceRepaint();
}
