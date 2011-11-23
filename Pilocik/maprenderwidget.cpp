#include "maprenderwidget.h"
#include "navigationwindow.h"

#include "qmath.h"
#include "settings.h"

#include <iostream>
#include <iomanip>

#include <QFileInfo>
#include <QDir>

#include <QObject>

#include <QDebug>

#include <QMouseEvent>
#include <QLineF>
#include <QDebug>
#include <QPointF>
#include <QPen>

#include <QtGui/QPixmap>
#include <QtGui/QApplication>

#include "twidgetmanager.h"
#include "widgets/thintwidget.h"

#include <osmscout/Database.h>
#include <osmscout/StyleConfigLoader.h>
#include <osmscout/Util.h>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Node.h>
#include <osmscout/Searching.h>

osmscout::Database *MapRenderWidget::database=0;
osmscout::DatabaseParameter MapRenderWidget::databaseParameter;

MapRenderWidget::MapRenderWidget(QWidget *parent,int width,int height):QWidget(parent){
    manualSimulation = false;
    movingPosition = false;
    lastNodeIndex = 0;

    //updateHint(NoHint);

    cachePixmapSize=2;
    delta=0.0;
    mouseDown=false;
    //Database loading
    if(database==0){
        QString map = Settings::getInstance()->getMapPath();
        if(map.size()==0) {
            std::cerr << "It's not a map, it's a shit!" << std::endl;
        }

        database = new osmscout::Database(databaseParameter);
        if (!database->Open((const char*)map.toAscii())) {
            std::cerr << "Fatal error: Cannot open database" << std::endl;
        }

    }

    QString style = Settings::getInstance()->getMapStylePath();
    styleConfig = new osmscout::StyleConfig(database->GetTypeConfig());
    if (!osmscout::LoadStyleConfig((const char*)style.toAscii(),*(styleConfig))) {
        std::cerr << "Cannot open style" << std::endl;
    }

    Settings* settings = Settings::getInstance();
    lat = settings->getLat();
    lon = settings->getLon();
    zoom = settings->getZoom();
    qDebug()<<lon<<":"<<lat<<":"<<zoom;
    projection.Set(lon,lat,lon,lat,0,zoom,0,0);

    setRouteSizes(zoom);

    rendererThread=new MapPixmapRenderer(this);
    qRegisterMetaType<osmscout::MercatorProjection>("osmscout::MercatorProjection");
    connect(rendererThread, SIGNAL(pixmapRendered(QImage,osmscout::MercatorProjection)), this, SLOT(newPixmapRendered(QImage,osmscout::MercatorProjection)));
    //testPixmap();

    searching = new osmscout::Searching();

    //connect(searching, SIGNAL(leaveRouteSig(double,double,double,double)), this, SLOT(leaveRoute(double,double,double,double)));

    //connect(this, SIGNAL(leaveRouteSig(double,double,double,double)), this, SLOT(leaveRoute(double,double,double,double)));

}


MapRenderWidget::~MapRenderWidget(){
    Settings::getInstance()->modifyMapSettings(projection.GetLat(), projection.GetLon(), projection.GetMagnification());
}

void MapRenderWidget::testPixmap(bool force){
    //qDebug()<<"111";
    bool needDraw=false;


    if(image.isNull())needDraw=true;
    else{
        double W=(projectionRendered.GetLonMax()-projectionRendered.GetLonMin())*delta/2;
        double H=(projectionRendered.GetLatMax()-projectionRendered.GetLatMin())*delta/2;
        if(projection.GetLonMin()<projectionRendered.GetLonMin()+W||
            projection.GetLonMax()>projectionRendered.GetLonMax()-W||
            projection.GetLatMin()<projectionRendered.GetLatMin()+H||
            projection.GetLatMax()>projectionRendered.GetLatMax()-H||
                force)
        {
                needDraw=true;
        }
    }

    if(needDraw&&!rendererThread->isWorking()){
            QSize s=this->size();
            projectionRendered1=projection;
            projectionRendered1.Set(projection.GetLon(), projection.GetLat(), lon, lat, 0, projection.GetMagnification()/cachePixmapSize, s.width()*cachePixmapSize, s.height()*cachePixmapSize);
            this->rendererThread->init((*(&database)),&projectionRendered1,(*(&styleConfig)),(float)cachePixmapSize);
            this->rendererThread->start();
    }
}

void MapRenderWidget::paintEvent(QPaintEvent *e){
    QSize s=this->size();
    projection.Set(projection.GetLon(),projection.GetLat(),projection.GetLon(),projection.GetLat(),projection.GetAngle(),projection.GetMagnification(),s.width(),s.height());
    testPixmap();
    if(!image.isNull()){
         QPainter *painter=new QPainter(this);

         double XMin,YMin;
         double XMax,YMax;

         projectionRendered.GeoToPixel(projection.GetLonMin(),projection.GetLatMin(),XMin,YMin);
         projectionRendered.GeoToPixel(projection.GetLonMax(),projection.GetLatMax(),XMax,YMax);

         double X,Y;
         projectionRendered.GeoToPixel(projection.GetLon(),projection.GetLat(),X,Y);

         QRectF source(XMin, YMax,XMax-XMin,YMin-YMax);
         //qDebug()<<XMin<<":"<<YMin<<":"<<XMax-XMin<<":"<<YMax-YMin;

         QRectF target(0,0,projection.GetWidth(),projection.GetHeight());
         //qDebug()<<"P"<<projection.GetWidth()<<":"<<projection.GetHeight();

         painter->drawImage ( target, image, source, Qt::AutoColor );

         //painter->drawImage(target,image);
         DrawPositionMarker(projection,painter);
         //qDebug()<<projection.GetWidth()/2-X<<":"<<projection.GetHeight()/2-Y<<":"<<pixmap.width()<<":"<<pixmap.height();;

         if (routing) {
             DrawRoute(projection, painter);
         }
         delete painter;
    }
}


void MapRenderWidget::mousePressEvent(QMouseEvent *e){
    mouseDown=true;
    lon1=e->posF().x();
    lat1=projection.GetHeight()-e->posF().y();

    double myX, myY;

    projection.GeoToPixel(myLon, myLat, myX, myY);
    if (manualSimulation && e->x() > myX - 10 && e->x() < myX + 10 && e->y() > myY - 10 && e->y() < myY + 10) {
        movingPosition = true;
    }
}

void MapRenderWidget::mouseReleaseEvent(QMouseEvent *e){
    mouseDown=false;
    movingPosition = false;
}

void MapRenderWidget::mouseMoveEvent(QMouseEvent *e){
    if(mouseDown){

        if (movingPosition) {

            int nextCross = getNextCrossIndex();

            if (nextCross != -1 && nextCross + 1 < route.size()) {
                QList<osmscout::Routing::RouteNode> ways;
                ways.append(route.at(nextCross + 1));

                nextIntersection = osmscout::Searching::SimulateNextCrossing(route.at(nextCross - 1),
                                                                             route.at(nextCross), ways);
            }

            QPointF correct = osmscout::Searching::CorrectPosition(route.at(lastNodeIndex),
                                                                   route.at(lastNodeIndex + 1),
                                                                   QPointF(e->x(), e->y()),
                                                                   projection);

            myLon = correct.x();
            myLat = correct.y();

            double distanceToEnd = -1;

            if (!route.isEmpty()) {
                distanceToEnd = floor(1000 * osmscout::Searching::CalculateDistance(myLon, myLat, route.last().lon, route.last().lat));
            }

            if (distanceToEnd >= 0 && distanceToEnd < 3) {
                updateHint(FinishRouteHint);
            //    qDebug() << "Finish hint " << distanceToEnd;
            } else {
                // niekoniecznie normal TODO
                updateHint(NormalHint);

            //    qDebug() << "Normal hint";
            }

            repaint();
        } else {

            double lon2,lat2,lon3,lat3,latR,lonR;

            projection.PixelToGeo(lon1,lat1,lon3,lat3);
            projection.PixelToGeo(e->posF().x(),projection.GetHeight()-e->posF().y(),lon2,lat2);
            lonR=lon3-lon2;
            latR=lat3-lat2;
            lon1=e->posF().x();
            lat1=projection.GetHeight()-e->posF().y();
            lonR+=projection.GetLon();
            latR+=projection.GetLat();

            projection.Set(lonR,latR,lonR,latR,0,projection.GetMagnification(),projection.GetWidth(),projection.GetHeight());
            testPixmap();
            repaint();
        }
    }
}

void MapRenderWidget::setCoordinates(double lonPar, double latPar){
    projection.Set(lonPar,latPar,projection.GetLon(),projection.GetLat(),0,projection.GetMagnification(),projection.GetWidth(),projection.GetHeight());
    repaint();
}

void MapRenderWidget::setMyCoordinates(double lonPar, double latPar,double angle){
    myLon=lonPar;
    myLat=latPar;
    myAngle=angle;

    if(tracking){
        setCoordinates(lonPar, latPar);
        this->update();
    }
}

QPointF MapRenderWidget::getCoordinates(){
    return QPointF(projection.GetLon(),projection.GetLat());
}

void MapRenderWidget::setRouteSizes(int zoom)
{
    if (zoom > 100000) {
        nodeR = 10;
        routeW = 10;
    }

    if (zoom < 100000) {
        nodeR = 8;
        routeW = 8;
    }

    if (zoom < 10000) {
        nodeR = 6;
        routeW = 6;
    }


    if (zoom < 1000) {
        nodeR = 4;
        routeW = 4;
    }

}

void MapRenderWidget::setZoom(int value){
    setRouteSizes(value);

    projection.Set(projection.GetLon(),projection.GetLat(),projection.GetLon(),projection.GetLat(),0,value,projection.GetWidth(),projection.GetHeight());
    testPixmap(true);
    repaint();
}

int MapRenderWidget::getZoom(){
    return projection.GetMagnification();
}

void MapRenderWidget::setRoute(QVector<osmscout::Routing::RouteNode> route)
{
    this->route = route;
}

QVector<osmscout::Routing::RouteNode> MapRenderWidget::getRoute()
{
    return route;
}

void MapRenderWidget::newPixmapRendered(QImage pixmap,osmscout::MercatorProjection projection){
    projectionRendered=projection;
    this->image=pixmap;
    testPixmap();
    repaint();
}

void MapRenderWidget::setStartZoom(int value){
}

void MapRenderWidget::setFinishZoom(int value){
    //projection.Set(projection.GetLon(),projection.GetLat(),projection.GetLon(),projection.GetLat(),0,value,projection.GetWidth(),projection.GetHeight());
    testPixmap(true);
    //repaint();
}

void MapRenderWidget::setTracking(bool tracking){
    this->tracking = tracking;
}

bool MapRenderWidget::getTracking(){
    return tracking;
}

void MapRenderWidget::setRouting(bool routing)
{
    this->routing = routing;
}

bool MapRenderWidget::getRouting()
{
    return routing;
}

int MapRenderWidget::getNextCrossIndex()
{
    for (int i = lastNodeIndex + 1; i < route.size(); i++) {
        if (route.at(i).crossing) {
            return i;
        }
    }

    return -1;
}

void MapRenderWidget::updateHint(HintType hintType)
{
    QString hintWidgetName = "Hint";

    THintWidget *hint = dynamic_cast<THintWidget*>(TWidgetManager::getInstance()->getWidget(hintWidgetName));

// CHECKING HINT TYPE
    if (hintType == NoHint) {
        TWidgetManager::getInstance()->setWidgetVisible(hintWidgetName, false);
    }

    if (hintType == NormalHint)
    {

        hint->setIntersection(nextIntersection);
        // next cross searching
        int nextCross = -1;
        for (int i = lastNodeIndex + 1; i < route.size(); i++) {
            if (route.at(i).crossing) {
                nextCross = i;
                break;
            }
        }

        // if is some cross on the route
        if (nextCross != -1) {

            // distance is a sum of all distances beetwen all nodes from lastNodeIndex to nextCross
            // probably move calculateDistance to other class
            //osmscout::Searching search;
            double distance = 0;

            // from actual position to lastNodeIndex + 1
            osmscout::Routing::RouteNode actNode;
            actNode.lat = myLat;
            actNode.lon = myLon;
            actNode.crossing = false;

            if (!osmscout::Searching::IsBetweenNodes(actNode, route.at(lastNodeIndex), route.at(lastNodeIndex + 1))) {
                for (int i = 0; i < route.size() - 1; i++) {
                    if (osmscout::Searching::IsBetweenNodes(actNode, route.at(i), route.at(i + 1))) {
                        lastNodeIndex = i;
                        break;
                    }
                }
            }

            //qDebug() << "Actual point is between: " << lastNodeIndex << " and " << lastNodeIndex + 1;

            distance += osmscout::Searching::CalculateDistance(actNode.lon, actNode.lat,
                                                               route.at(lastNodeIndex + 1).lon,
                                                               route.at(lastNodeIndex + 1).lat);

            // we are sure that lastNodeIndex < nextCross && nextCross < route.size
            for (int i = lastNodeIndex + 1; i < nextCross; i++) {
                distance += osmscout::Searching::CalculateDistance(route.at(i).lon,
                                                     route.at(i).lat,
                                                     route.at(i + 1).lon,
                                                     route.at(i + 1).lat);
            }

            distance = floor(1000 * distance);
            hint->updateDistance(distance);

            //hint->drawIntersection(nextIntersection);
        }


    }

    if (hintType == LeaveRouteHint)
    {
        hint->setLeaveRoute();
    }

    if (hintType == FinishRouteHint)
    {
        hint->setFinishRoute();
    }

}

void MapRenderWidget::leaveRoute(double actLon, double actLat, double destLon, double destLat)
{

}

QPen MapRenderWidget::setPenStyle(PenStyle penStyle)
{
    QPen pen;

    switch (penStyle) {
    case NORMAL_LINE: // normal line
        pen.setColor(Qt::black);
        pen.setWidth(1);
    break;
    case ROUTE_LINE:
        pen.setColor(QColor(0, 0, 255, 127));
        pen.setWidthF(routeW);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCosmetic(true);
    break;
    }

    return pen;
}

void MapRenderWidget::DrawRoute(const osmscout::Projection &projection, QPainter *painter)
{
    manualSimulation = true;
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = lastNodeIndex + 1; i < route.size() - 1; i++) {
        osmscout::Routing::RouteNode node = route.at(i);
        osmscout::Routing::RouteNode nextNode = route.at(i + 1);

        double x, x2, y, y2;
        projection.GeoToPixel(node.lon, node.lat, x, y);
        projection.GeoToPixel(nextNode.lon, nextNode.lat, x2, y2);

        //qDebug() << x << " " << y;

        QPointF p1(x, y);
        QPointF p2(x2, y2);

        painter->setPen(setPenStyle(ROUTE_LINE));
        painter->drawLine(p1, p2);

        painter->setBrush(QBrush(Qt::red));

        // TEMP
        if (i == 0) {
            //painter->setBrush(QBrush(Qt::red));
           //painter->drawEllipse(x-7, y-7, 15, 15);

            //setMyCoordinates(node.lon, node.lat, 90);
            //myLon = node.lon;
            //myLat = node.lat;

            //lastNodeIndex = i;

        //    updateHint();
        }
    }

    if (lastNodeIndex < route.size() ) {
        osmscout::Routing::RouteNode lastNode = route.at(lastNodeIndex);

        double actX, actY, lastX, lastY;
        projection.GeoToPixel(myLon, myLat, actX, actY);
        projection.GeoToPixel(lastNode.lon, lastNode.lat, lastX, lastY);


        double nextX, nextY;
        projection.GeoToPixel(route.at(lastNodeIndex + 1).lon, route.at(lastNodeIndex + 1).lat, nextX, nextY);

        painter->setPen(setPenStyle(ROUTE_LINE));
        painter->drawLine(QPointF(actX, actY), QPointF(nextX, nextY));

        painter->setPen(setPenStyle());
        painter->setBrush(Qt::red);
        painter->drawEllipse(nextX - nodeR, nextY - nodeR, 2*nodeR, 2*nodeR);
    }

    if (!route.isEmpty()) {
        osmscout::Routing::RouteNode finishNode = route.last();
        double finishX, finishY;
        projection.GeoToPixel(finishNode.lon, finishNode.lat, finishX, finishY);

        painter->setPen(setPenStyle());
        painter->setBrush(Qt::green);
        painter->drawEllipse(finishX - nodeR, finishY - nodeR, 2*nodeR, 2*nodeR);
    }
    //updateHint(NormalHint);
}

void MapRenderWidget::DrawPositionMarker(const osmscout::Projection& projection,QPainter *painter)
{
  double x,y;
  QPolygon marker;
  QMatrix matrix, matrix2, result;

  marker << QPoint(-10,8) << QPoint(10,8) << QPoint(0,-15);
  projection.GeoToPixel(myLon,myLat, x, y);

  matrix.rotate(myAngle);
  matrix2.translate((int)x, (int)y);

  result = matrix.operator *(matrix2);
  marker = result.map(marker);

  QPointF g1(0,-0.5);
  QPointF g2(0,0.5);
  matrix2.reset();
  matrix2.translate(0.5,0.5);
  result.reset();
  result = matrix.operator *(matrix2);
  g1 = result.map(g1);
  g2 = result.map(g2);

  QLinearGradient grad = QLinearGradient(g1,g2);
  QGradientStops stops;
  stops << QGradientStop(0, QColor::fromRgb(0x3b679e));
  stops << QGradientStop(0.6, QColor::fromRgb(0x2b88d9));
  stops << QGradientStop(0.61, QColor::fromRgb(0x207cca));
  stops << QGradientStop(1, QColor::fromRgb(0x7db9e8));
  grad.setStops(stops);
  grad.setCoordinateMode(QGradient::ObjectBoundingMode);

  painter->setBrush(QBrush(grad));
  painter->setPen(QPen(QColor::fromRgb(255,255,255,255)));
  painter->drawPolygon(marker);
}



//////////////////////////////////////////////////////////////

MapPixmapRenderer::MapPixmapRenderer(QObject *parent):QThread(parent){
    mapPainter=new osmscout::MapPainterQt();
    started=false;

}

void MapPixmapRenderer::getPixmap(){

}

void MapPixmapRenderer::init(osmscout::Database *database,osmscout::MercatorProjection  *projection,osmscout::StyleConfig*style,float d){
    this->database=database;
    this->projection=projection;
    this->d=d;
    styleConfig=style;
}

void MapPixmapRenderer::run(){
    //while(){
        //render pixmap
    started=true;
    qDebug()<<"tak";
        QSize size(projection->GetWidth(),projection->GetHeight());
        //TODO: TEST FORMATS
        QImage pixmap(size,QImage::Format_RGB16);
        qDebug()<<"LL"<<projection->GetWidth();
        osmscout::MapData             data;
        osmscout::MapParameter        drawParameter;
        osmscout::AreaSearchParameter searchParameter;
        QPainter *painter=new QPainter(&pixmap);
        drawParameter.SetOptimizeAreaNodes(true);
        drawParameter.SetOptimizeWayNodes(true);
        database->GetObjects(*(styleConfig),
                            projection->GetLonMin(),
                            projection->GetLatMin(),
                            projection->GetLonMax(),
                            projection->GetLatMax(),
                            projection->GetMagnification()*d,
                            searchParameter,
                            data.nodes,
                            data.ways,
                            data.areas,
                            data.relationWays,
                            data.relationAreas);
        mapPainter->DrawMap(*(styleConfig),
                               (*projection),
                               drawParameter,
                               data,
                               painter );
        osmscout::MercatorProjection p=(*projection);
        pixmapRendered(pixmap,p);
        delete painter;
    started=false;
}


bool MapPixmapRenderer::isWorking(){
    return started;
}
