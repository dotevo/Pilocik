#include "maprenderwidget.h"
#include "navigationwindow.h"

#include "qmath.h"
#include "gpsreceiver.h"
#include "settings.h"

#include <iostream>
#include <iomanip>

#include <QFileInfo>
#include <QDir>

#include <QMouseEvent>
#include <QLineF>
#include <QDebug>

#include <QtGui/QPixmap>
#include <QtGui/QApplication>

#include <osmscout/Database.h>
#include <osmscout/StyleConfigLoader.h>
#include <osmscout/Util.h>
#include <osmscout/MapPainterQt.h>
#include <osmscout/Node.h>

osmscout::Database *MapRenderWidget::database=0;
osmscout::DatabaseParameter MapRenderWidget::databaseParameter;

MapRenderWidget::MapRenderWidget(QWidget *parent,int width,int height):QWidget(parent){
    cachePixmapSize=5;
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


    lat = 51.1;
    lon = 17.03;
    zoom = 2*2*2*1024;

    //Set projection
    projection.Set(lon, lat, lon, lat, 0, zoom, 1000, 1000);


    rendererThread=new MapPixmapRenderer();
    qRegisterMetaType<osmscout::MercatorProjection>("osmscout::MercatorProjection");
    connect(rendererThread, SIGNAL(pixmapRendered(QPixmap,osmscout::MercatorProjection)), this, SLOT(newPixmapRendered(QPixmap,osmscout::MercatorProjection)));
    testPixmap();
}

void MapRenderWidget::testPixmap(){
    //qDebug()<<"111";
    bool needDraw=false;

    if(pixmap.isNull())needDraw=true;
    else if(projection.GetLonMin()<projectionRendered.GetLonMin()||
            projection.GetLonMax()>projectionRendered.GetLonMax()||
            projection.GetLatMin()<projectionRendered.GetLatMin()||
            projection.GetLatMax()>projectionRendered.GetLatMax()){
                needDraw=true;
                qDebug()<<"[][]["<<projection.GetHeight();
                qDebug()<<"NEEDED!!";
               /* qDebug()<<projection.GetLonMin()<<":"<<projectionRendered.GetLonMin()<<";\n"<<
                          projection.GetLonMax()<<":"<<projectionRendered.GetLonMax()<<";\n"<<
                          projection.GetLatMin()<<":"<<projectionRendered.GetLatMin()<<";\n"<<
                          projection.GetLatMax()<<":"<<projectionRendered.GetLatMax();

                          */
    }

    //qDebug()<<"222";
    if(needDraw){
            QSize s=this->size();
            projectionRendered1=projection;
            projectionRendered1.Set(projection.GetLon(), projection.GetLat(), lon, lat, 0, zoom/cachePixmapSize, s.width()*cachePixmapSize, s.height()*cachePixmapSize);
            //qDebug()<<projectionRendered1.GetWidth()<<"{}"<<s.width();
            this->rendererThread->init((*(&database)),&projectionRendered1,(*(&styleConfig)));
            this->rendererThread->start();
    }
}



void MapRenderWidget::paintEvent(QPaintEvent *e){
    QSize s=this->size();
    projection.Set(projection.GetLon(),projection.GetLat(),projection.GetLon(),projection.GetLat(),projection.GetAngle(),projection.GetMagnification(),s.width(),s.height());
    testPixmap();
    if(!pixmap.isNull()){
         QPainter *painter=new QPainter(this);
         double X,Y;
         projectionRendered.GeoToPixel(projection.GetLon(),projection.GetLat(),X,Y);
         //qDebug()<<X<<"[]"<<Y;
         painter->drawPixmap(projection.GetWidth()/2-X,projection.GetHeight()/2-Y,pixmap.width(),pixmap.height(),pixmap);
         //qDebug()<<projection.GetWidth()/2-X<<":"<<projection.GetHeight()/2-Y<<":"<<pixmap.width()<<":"<<pixmap.height();
         delete painter;
    }
}

//TODO: REALNE PRZESUWANIE!!! Na projekcji

void MapRenderWidget::mousePressEvent(QMouseEvent *e){

}

void MapRenderWidget::mouseReleaseEvent(QMouseEvent *e){
    double lon1,lat1;
    projection.PixelToGeo(e->posF().x(),projection.GetHeight()-e->posF().y(),lon1,lat1);
    qDebug()<<"PROJ"<<projection.GetWidth()<<"PIXELTOGEO:"<<e->posF().x()<<":"<<e->posF().y()<<":"<<lon1<<":"<<lat1<<"KK"<<projection.GetLon()<<":"<<projection.GetLat();

    projection.Set(lon1,lat1,lon1,lat1,0,projection.GetMagnification(),projection.GetWidth(),projection.GetHeight());
    testPixmap();
    repaint();
}

void MapRenderWidget::mouseMoveEvent(QMouseEvent *e){
}
////////////////////////////////

void MapRenderWidget::newPixmapRendered(QPixmap pixmap,osmscout::MercatorProjection projection){
    qDebug()<<"AAA";
    projectionRendered=projection;
    this->pixmap=pixmap;
    repaint();
}

//////////////////////////////////////////////////////////////

MapPixmapRenderer::MapPixmapRenderer(){
    mapPainter=new osmscout::MapPainterQt();
    started=false;
}

void MapPixmapRenderer::getPixmap(){

}

void MapPixmapRenderer::init(osmscout::Database *database,osmscout::MercatorProjection  *projection,osmscout::StyleConfig*style){
    this->database=database;
    this->projection=projection;
    styleConfig=style;
}

void MapPixmapRenderer::run(){
    //while(){
        //render pixmap
    if(started)return;
    started=true;
        QPixmap *pixmap=new QPixmap(projection->GetWidth(),projection->GetHeight());
        qDebug()<<"LL"<<projection->GetWidth();
        osmscout::MapData             data;
        osmscout::MapParameter        drawParameter;
        osmscout::AreaSearchParameter searchParameter;
        QPainter *painter=new QPainter(pixmap);
        drawParameter.SetOptimizeAreaNodes(true);
        drawParameter.SetOptimizeWayNodes(true);
        database->GetObjects(*(styleConfig),
                            projection->GetLonMin(),
                            projection->GetLatMin(),
                            projection->GetLonMax(),
                            projection->GetLatMax(),
                            projection->GetMagnification(),
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
        pixmapRendered(*(pixmap),p);
        delete painter;
    started=false;
}


/*

void DrawPositionMarker(const Projection& projection);
void MapPainterQt::DrawPositionMarker(const Projection& projection)
{
  double x,y;
  QPolygon marker;
  QMatrix matrix, matrix2, result;

  marker << QPoint(-10,8) << QPoint(10,8) << QPoint(0,-15);
  projection.GeoToPixel(projection.GetMarkerLon(),projection.GetMarkerLat(), x, y);

  matrix.rotate(projection.GetAngle());
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

*/
