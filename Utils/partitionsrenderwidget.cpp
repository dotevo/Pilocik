#include "partitionsrenderwidget.h"
#include <QDebug>

PartitionsRenderWidget::PartitionsRenderWidget(QWidget *parent) :
    QWidget(parent)
{
}

void PartitionsRenderWidget::init(QString dbPath)
{
    width = this->size().width();
    height = this->size().height();

    showNodes = true;
    showWays = true;
    mapDrag = false;

    pixmap = QPixmap(width, height);

    calculateNewMapCenter(17.03,51.11,30);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open()){
        qDebug()<<"[SQLiteDatabase::open]"<<db.lastError();
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id,lon,lat,cell FROM nodes");
    query.exec();
    while (query.next()) {
        pNode newNode;
        newNode.lon = query.value(1).toDouble();
        newNode.lat = query.value(2).toDouble();
        newNode.cellNo = query.value(3).toInt();
        pNodes.insert(query.value(0).toInt(), newNode);
    }

    query.prepare("SELECT id, prio_car FROM ways");
    query.exec();
    while (query.next()) {
        pWay newWay;
        newWay.priority = query.value(1).toInt();
        pWays.insert(query.value(0).toInt(), newWay);
    }

    query.prepare("SELECT way, node, num FROM ways_nodes");
    query.exec();
    while (query.next()) {
        pWays[query.value(0).toInt()].nodes.append(query.value(1).toInt());
    }

    db.close();

    int cells = 50;

    // Generate colors(one color for each cell)
    for(int i = 0; i < cells; i++){
      QColor myColor;
      myColor = QColor::fromHsvF(1/(double)cells*i, 0.6, 0.95);
      pColors.append(myColor);
    }

    // Mix colors in list
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    for(int i = 0; i < 300; i++)
    {
        pColors.swap(qrand()%cells,qrand()%cells);
    }

//!   Display generated colors
//    for(int i = 0; i < colors; i++){
//      painter->setPen((new QColor())->fromRgb(255,255,255));
//      painter->setBrush(colorTab.at(i));
//      QRect rect = QRect(QPoint(i*10,0),QPoint(i*10+10,20));
//      painter->drawRect(rect);
//    }

    repaint();
}

void PartitionsRenderWidget::calculateNewMapCenter(double lon, double lat, double magnification)
{
    this->lat = lat;
    this->lon = lon;
    this->magnification = magnification;

    boxWidth=360/magnification;
    boxHeight=boxWidth*height/width;

    lonMin=lon-boxWidth/2;
    lonMax=lon+boxWidth/2;


    latMin=atan(sinh(atanh(sin(lat*gradtorad))-boxHeight/2*gradtorad))/gradtorad;
    latMax=atan(sinh(atanh(sin(lat*gradtorad))+boxHeight/2*gradtorad))/gradtorad;

    hmin=lonMin*gradtorad;
    hmax=lonMax*gradtorad;
    vmin=atanh(sin(latMin*gradtorad));
    vmax=atanh(sin(latMax*gradtorad));

    hscale=(width-1)/(hmax-hmin);
    vscale=(height-1)/(vmax-vmin);
}

void PartitionsRenderWidget::paintEvent(QPaintEvent *e)
{
    double dx = 0;
    double dy = 0;
    if(!mapDrag){
        renderPartitions();
    }
    else{
        QLineF line(dragStart, dragMove);
        dx = line.dx();
        dy = line.dy();
    }
    QPainter* painter= new QPainter(this);
    painter->drawPixmap(dx,dy,pixmap.width(),pixmap.height(),pixmap);
    delete painter;
}

void PartitionsRenderWidget::mousePressEvent(QMouseEvent *e)
{
    dragStart = e->globalPos();
    dragMove = dragStart;
    mapDrag = true;
}

void PartitionsRenderWidget::mouseMoveEvent(QMouseEvent *e)
{
    mapDrag = true;
    dragMove = e->globalPos();
    repaint();
}

void PartitionsRenderWidget::mouseReleaseEvent(QMouseEvent *e)
{
    dragEnd = e->globalPos();
    QLineF line(dragStart, dragEnd);
    dragStart = dragEnd;

    mapDrag = false;

    double dx = line.dx();
    double dy = line.dy();

    double lon,lat;

    pixelToGeo(width/2-dx, height/2+dy, lon, lat);

    calculateNewMapCenter(lon, lat, magnification);

    repaint();
}

void PartitionsRenderWidget::resizeEvent(QResizeEvent *e)
{
    width = size().width();
    height = size().height();
}

void PartitionsRenderWidget::renderPartitions()
{
    pixmap = QPixmap(width, height);
    QPainter *painter = new QPainter(&pixmap);
    painter->fillRect(0,0,width,height,QColor(255,255,255));
    //Define size
    double size = magnification/120;
    // Display nodes
    if(showNodes)
    foreach(pNode myNode, pNodes.values())
    {

        painter->setPen(pColors.at(myNode.cellNo%50));
        painter->setBrush(pColors.at(myNode.cellNo%50));
        double x, y;
        geoToPixel(myNode.lon, myNode.lat, x, y);
        painter->drawEllipse(QPointF(x,y),size,size);
    }

    // Display ways
    if(showWays)
    foreach(pWay myWay, pWays.values())
    {
        for(int i = 0; i < myWay.nodes.size()-1; i++)
        {
            pNode currNode = pNodes[myWay.nodes.at(i)];
            pNode nextNode = pNodes[myWay.nodes.at(i+1)];

            QPen pen;

            pen.setColor(pColors.at(currNode.cellNo%50));
            pen.setWidthF(size);
            pen.setJoinStyle(Qt::RoundJoin);
            pen.setCapStyle(Qt::RoundCap);
            pen.setStyle(Qt::SolidLine);

            painter->setPen(pen);
            double x1, y1, x2, y2;
            geoToPixel(currNode.lon, currNode.lat, x1, y1);
            geoToPixel(nextNode.lon, nextNode.lat, x2, y2);
            QPainterPath path(QPointF(x1,y1));
            path.lineTo(x2,y2);
            painter->setPen(pen);
            painter->drawPath(path);
        }
    }
    delete painter;
}

void PartitionsRenderWidget::changeZoom(int zoom)
{
    calculateNewMapCenter(lon, lat, zoom);
    repaint();
}

void PartitionsRenderWidget::showWaysChange(int b)
{
    showWays = b==2;
    repaint();
}

void PartitionsRenderWidget::showNodesChange(int b)
{
    showNodes = b==2;
    repaint();
}

void PartitionsRenderWidget::geoToPixel(double lon, double lat,
                                    double& x, double& y)
{
    x=(lon*gradtorad-hmin)*hscale;
    y=height-(atanh(sin(lat*gradtorad))-vmin)*vscale;
}

void PartitionsRenderWidget::pixelToGeo(double x, double y,
                                    double& lon, double& lat)
{
    lon=lonMin+(lonMax-lonMin)*x/width;
    lat=latMin+(latMax-latMin)*y/height;
}
