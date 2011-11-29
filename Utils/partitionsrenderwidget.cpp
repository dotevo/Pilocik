#include "partitionsrenderwidget.h"
#include <QDebug>

#include <../../PiLibocik/include/pilibocik/geohash.h>
#include <../../PiLibocik/include/pilibocik/boundarybox.h>
#include <../../PiLibocik/include/pilibocik/partition/boundaryedge.h>
#include <../../PiLibocik/include/pilibocik/partition/partitionfile.h>
#include <../../PiLibocik/include/pilibocik/partition/node.h>
#include <../../PiLibocik/include/pilibocik/partition/way.h>

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
    showBoundary = true;
    mapDrag = false;

    pixmap = QPixmap(width, height);

    calculateNewMapCenter(17.03,51.11,130);
    //calculateNewMapCenter(2, 2, 15); // for testing graphs

    bool fromDatabase = false;
    if(fromDatabase) {
        // partition from database
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

        query.prepare("SELECT id, prio_car FROM innerWays");
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

        query.prepare("SELECT node1, node2, wayId FROM boundaryEdges");
        query.exec();
        while (query.next()) {
            pBoundary pb;
            pb.node1 = query.value(0).toInt();
            pb.node2 = query.value(1).toInt();
            pb.way = query.value(2).toInt();
            pBoundaryWays.append(pb);
        }

        db.close();
    } else {
        // partition loaded from binary files
        PiLibocik::Partition::PartitionFile partitionFile(dbPath, "car", QIODevice::ReadOnly, 1);

        PiLibocik::BoundaryBox bbox(PiLibocik::Position(1.1, 0.1), PiLibocik::Position(62.0, 62.0));
        QList<PiLibocik::Partition::Node> fileNodes = partitionFile.getNodesFromBoundaryBox(bbox);
        QListIterator<PiLibocik::Partition::Node> fileNodesIter(fileNodes);

        while(fileNodesIter.hasNext()){
            PiLibocik::Partition::Node fileNode = fileNodesIter.next();

            pNode newNode;
            newNode.lon = fileNode.getLon();
            newNode.lat = fileNode.getLat();
            newNode.cellNo = fileNode.getCell();
            pNodes.insert(fileNode.getId(), newNode);

            QVector<PiLibocik::Partition::Way> fileWays = fileNode.getWaysObj();
            for(int i=0; i<fileWays.size(); i++){
                PiLibocik::Partition::Way fileWay = fileWays.at(i);

                if(pWays.find(fileWay.getId()) == pWays.end()) {
                    pWay newWay;
                    newWay.priority = fileWay.getPrioritet();
                    pWays.insert(fileWay.getId(), newWay);

                    QVector<PiLibocik::Partition::Node> fileNodesInWay = fileWay.getNodesObj();
                    for(int j=0;j<fileNodesInWay.size();j++){
                        PiLibocik::Partition::Node fileNodeInWay = fileNodesInWay.at(j);

                        pWays[fileWay.getId()].nodes.append(fileNodeInWay.getId());
                    }
                }
            }

            QVector<PiLibocik::Partition::BoundaryEdge> fileBoundaryEdges = fileNode.getBoundaryEdges();
            for(int i=0; i<fileBoundaryEdges.size(); i++){
                PiLibocik::Partition::BoundaryEdge fileBoundaryEdge = fileBoundaryEdges.at(i);

                PiLibocik::Partition::Node filePairNode = fileBoundaryEdge.getPairObj();
                if(filePairNode.getId() > fileNode.getId()) {
                    pBoundary pb;
                    pb.node1 = fileNode.getId();
                    pb.node2 = filePairNode.getId();
                    pb.way = fileBoundaryEdge.getWayObj().getId();
                    pBoundaryWays.append(pb);
                }
            }
        }
    }

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
    painter->fillRect(0,0,width,height,QColor(230,230,230));
    //Define size
    double size = magnification/240;
    // Display nodes
    if(showNodes)
    foreach(pNode myNode, pNodes.values())
    {
        painter->setPen(pColors.at(myNode.cellNo%50));
        painter->setBrush(pColors.at(myNode.cellNo%50));
        double x, y;
        geoToPixel(myNode.lon, myNode.lat, x, y);
        painter->drawEllipse(QPointF(x,y),2,2);
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

    if(showBoundary)
    foreach(pBoundary myBoundary, pBoundaryWays){
        pNode node1 = pNodes[myBoundary.node1];
        pNode node2 = pNodes[myBoundary.node2];

        QPen pen;

        pen.setColor(QColor(0,0,0));
        pen.setWidthF(1);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setCapStyle(Qt::RoundCap);
        pen.setStyle(Qt::DotLine);

        painter->setPen(pen);
        double x1, y1, x2, y2;
        geoToPixel(node1.lon, node1.lat, x1, y1);
        geoToPixel(node2.lon, node2.lat, x2, y2);
        QPainterPath path(QPointF(x1,y1));
        path.lineTo(x2,y2);
        painter->setPen(pen);
        painter->drawPath(path);
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

void PartitionsRenderWidget::showBoundaryEdges(int b)
{
    showBoundary = b==2;
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
