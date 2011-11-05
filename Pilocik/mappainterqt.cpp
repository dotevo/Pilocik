/*
  Class from library osmscout. It's not documented :-)
*/

#include <osmscout/MapPainterQt.h>

#include <cassert>
#include <iostream>
#include <limits>

#include <QFile>
#include <QTime>

#include <osmscout/util/Geometry.h>

#include <osmscout/private/Math.h>

namespace osmscout {

  MapPainterQt::MapPainterQt()
  {
    sin.resize(360*10);

    for (size_t i=0; i<sin.size(); i++) {
      sin[i]=std::sin(M_PI/180*i/(sin.size()/360));
    }

    images = new std::vector<QImage>();
  }

  MapPainterQt::~MapPainterQt()
  {
    // no code
    // TODO: Clean up fonts
  }

  QFont MapPainterQt::GetFont(const MapParameter& parameter,
                              double fontSize)
  {
    std::map<size_t,QFont>::const_iterator f;

    f=fonts.find(fontSize);

    if (f!=fonts.end()) {
      return f->second;
    }

    QFont font(parameter.GetFontName().c_str(),QFont::Normal,false);

    font.setPixelSize(parameter.GetFontSize()*fontSize);
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setStyleStrategy(QFont::PreferMatch);

    return fonts.insert(std::pair<size_t,QFont>(fontSize,font)).first->second;
  }

  bool MapPainterQt::HasIcon(const StyleConfig& styleConfig,
                             const MapParameter& parameter,
                             IconStyle& style)
  {
      if (images->size() != 24) {
        if (style.GetId()==std::numeric_limits<size_t>::max()) {
          return false;
        }

        if (style.GetId()!=0) {
          return true;
        }

        std::list<std::string> iconsPaths;
        iconsPaths.push_back(":/png/images/icons/14x14/standard/");

        //for (std::list<std::string>::const_iterator path=parameter.GetIconPaths().begin();
        for (std::list<std::string>::const_iterator path=iconsPaths.begin();
             path!=iconsPaths.end();
             ++path) {

          std::string filename=*path+style.GetIconName()+".png";

          QImage image;

          if (image.load(filename.c_str())) {
            images->resize(images->size()+1,image);
            style.SetId(images->size());
            //std::cout << "Loaded image " << filename << " => id " << style.GetId() << std::endl;

            return true;
          }
        }
      }

    std::cerr << "ERROR while loading icon file '" << style.GetIconName() << "'" << std::endl;
    style.SetId(std::numeric_limits<size_t>::max());

    return false;
  }

  bool MapPainterQt::HasPattern(const MapParameter& parameter,
                                const FillStyle& style)
  {
    assert(style.HasPattern());

    // Was not able to load pattern
    if (style.GetPatternId()==std::numeric_limits<size_t>::max()) {
      return false;
    }

    // Pattern already loaded
    if (style.GetPatternId()!=0) {
      return true;
    }

    std::list<std::string> patternPaths;
    patternPaths.push_back(":/png/images/icons/14x14/standard/");

    //for (std::list<std::string>::const_iterator path=parameter.GetPatternPaths().begin();
    for (std::list<std::string>::const_iterator path = patternPaths.begin();
         path != patternPaths.end();
         ++path) {
      std::string filename=*path+style.GetPatternName()+".png";

      QImage image;

      if (image.load(filename.c_str())) {
        images->resize(images->size()+1,image);
        style.SetPatternId(images->size());
        patterns.resize(images->size());

        patterns[patterns.size()-1].setTextureImage(image);

        //std::cout << "Loaded image " << filename << " => id " << style.GetPatternId() << std::endl;

        return true;
      }
    }

    std::cerr << "ERROR while loading icon 2 file '" << style.GetPatternName() << "'" << std::endl;
    style.SetPatternId(std::numeric_limits<size_t>::max());

    return false;
  }

  void MapPainterQt::GetTextDimension(const MapParameter& parameter,
                                      double fontSize,
                                      const std::string& text,
                                      double& xOff,
                                      double& yOff,
                                      double& width,
                                      double& height)
  {
    QFont        font(GetFont(parameter,fontSize));
    QFontMetrics metrics=QFontMetrics(font);
    QString      string=QString::fromUtf8(text.c_str());
    QRect        extents=metrics.boundingRect(string);

    xOff=extents.x();
    yOff=extents.y();
    width=extents.width();
    height=extents.height();
  }

  void MapPainterQt::DrawLabel(const Projection& projection,
                               const MapParameter& parameter,
                               const Label& label)
  {
    double r=label.style->GetTextR();
    double g=label.style->GetTextG();
    double b=label.style->GetTextB();

    QFont        font(GetFont(parameter,label.fontSize));
    QString      string=QString::fromUtf8(label.text.c_str());
    QFontMetrics metrics=QFontMetrics(font);

    if (label.style->GetStyle()==LabelStyle::normal) {
      painter->setPen(QColor::fromRgbF(r,g,b,label.alpha));
      painter->setBrush(Qt::NoBrush);
      painter->setFont(font);
      painter->drawText(QPointF(label.x,
                                label.y+metrics.ascent()),
                        string);
    }
    else if (label.style->GetStyle()==LabelStyle::emphasize) {
      QPainterPath path;
      QPen         pen;

      pen.setColor(QColor::fromRgbF(1.0,1.0,1.0,label.alpha));
      pen.setWidthF(2.0);
      painter->setPen(pen);

      path.addText(QPointF(label.x,
                           label.y+metrics.ascent()),
                           font,
                   string);

      painter->drawPath(path);
      painter->fillPath(path,QBrush(QColor::fromRgbF(r,g,b,label.alpha)));
    }
  }

  void MapPainterQt::DrawPlateLabel(const Projection& projection,
                                    const MapParameter& parameter,
                                    const Label& label)
  {
    QFont        font(GetFont(parameter,label.fontSize));
    QFontMetrics metrics=QFontMetrics(font);
    QString      string=QString::fromUtf8(label.text.c_str());

    painter->fillRect(QRectF(label.bx,
                             label.by,
                             label.bwidth,
                             label.bheight),
                      QBrush(QColor::fromRgbF(label.style->GetBgR(),
                                              label.style->GetBgG(),
                                              label.style->GetBgB(),
                                              1)));

    painter->setPen(QColor::fromRgbF(label.style->GetBorderR(),
                                     label.style->GetBorderG(),
                                     label.style->GetBorderB(),
                                     label.style->GetBorderA()));
    painter->setBrush(Qt::NoBrush);

    painter->drawRect(QRectF(label.bx+2,
                             label.by+2,
                             label.bwidth-4,
                             label.bheight-4));

    painter->setPen(QColor::fromRgbF(label.style->GetTextR(),
                                     label.style->GetTextG(),
                                     label.style->GetTextB(),
                                     label.style->GetTextA()));
    painter->setBrush(Qt::NoBrush);
    painter->setFont(font);
    painter->drawText(QPointF(label.x,
                              label.y+metrics.ascent()),
                              string);
  }

  void MapPainterQt::DrawContourLabel(const Projection& projection,
                                      const MapParameter& parameter,
                                      const LabelStyle& style,
                                      const std::string& text,
                                      size_t transStart, size_t transEnd)
  {
    double fontSize=style.GetSize();
    double r=style.GetTextR();
    double g=style.GetTextG();
    double b=style.GetTextB();
    double a=style.GetTextA();

    QPen          pen;
    QFont         font(GetFont(parameter,fontSize));
    QFontMetricsF metrics=QFontMetricsF(font,painter->device());
    QString       string=QString::fromUtf8(text.c_str());
    double        stringLength=metrics.width(string);

    pen.setColor(QColor::fromRgbF(r,g,b,a));
    painter->setPen(pen);
    painter->setFont(font);

    QPainterPath p;

    if (transBuffer.buffer[transStart].x<transBuffer.buffer[transEnd].x) {
      for (size_t j=transStart; j<=transEnd; j++) {
        if (j==transStart) {
          p.moveTo(transBuffer.buffer[j].x,transBuffer.buffer[j].y);
        }
        else {
          p.lineTo(transBuffer.buffer[j].x,transBuffer.buffer[j].y);
        }
      }
    }
    else {
      for (size_t j=0; j<=transEnd-transStart; j++) {
        size_t idx=transEnd-j;

        if (j==0) {
          p.moveTo(transBuffer.buffer[idx].x,
                   transBuffer.buffer[idx].y);
        }
        else {
          p.lineTo(transBuffer.buffer[idx].x,
                   transBuffer.buffer[idx].y);
        }
      }
    }

    if (p.length()<stringLength) {
      // Text is longer than path to draw on
      return;
    }

    qreal offset=(p.length()-stringLength)/2;

    QTransform tran;

    for (int i=0; i<string.size(); i++) {
      QPointF point=p.pointAtPercent(p.percentAtLength(offset));
      qreal angle=p.angleAtPercent(p.percentAtLength(offset));

      // rotation matrix components

      qreal sina=sin[lround((360-angle)*10)%sin.size()];
      qreal cosa=sin[lround((360-angle+90)*10)%sin.size()];

      // Rotation
      qreal newX=(cosa*point.x())-(sina*point.y());
      qreal newY=(cosa*point.y())+(sina*point.x());

      // Aditional offseting
      qreal deltaPenX=cosa*pen.width();
      qreal deltaPenY=sina*pen.width();

      // Getting the delta distance for the translation part of the transformation
      qreal deltaX=newX-point.x();
      qreal deltaY=newY-point.y();

      // Applying rotation and translation.
      tran.setMatrix(cosa,sina,0.0,
                     -sina,cosa,0.0,
                     -deltaX+deltaPenX,-deltaY-deltaPenY,1.0);

      painter->setTransform(tran);

      painter->drawText(point,QString(string[i]));

      offset+=metrics.width(string[i]);
    }

    painter->resetTransform();
  }

  void MapPainterQt::DrawIcon2(const IconStyle* style,
                              double x, double y)
  {
    assert(style->GetId()>0);
    assert(style->GetId()!=std::numeric_limits<size_t>::max());
    assert(style->GetId()<=images->size());

    //assert(!images[style->GetId()-1].isNull());
    assert(!images->at(style->GetId()-1).isNull());


/*
painter->drawImage(QPointF(x-images[style->GetId()-1].width()/2,
                               y-images[style->GetId()-1].height()/2),
                       images[style->GetId()-1]);
*/
    painter->drawImage(QPointF(x-images->at(style->GetId()-1).width()/2,
                                   y-images->at(style->GetId()-1).height()/2),
                           images->at(style->GetId()-1));
  }

  void MapPainterQt::DrawSymbol(const SymbolStyle* style, double x, double y)
  {
    QPainterPath path;

    switch (style->GetStyle()) {
    case SymbolStyle::none:
      break;
    case SymbolStyle::box:
      path.addRect(x-style->GetSize()/2,y-style->GetSize()/2,
                   style->GetSize(),style->GetSize());
      painter->fillPath(path,QBrush(QColor::fromRgbF(style->GetFillR(),
                                                     style->GetFillG(),
                                                     style->GetFillB(),
                                                     style->GetFillA())));
      break;
    case SymbolStyle::circle:
      path.addEllipse(QPointF(x,y),
                      (double)style->GetSize(),
                      (double)style->GetSize());
      painter->fillPath(path,QBrush(QColor::fromRgbF(style->GetFillR(),
                                                     style->GetFillG(),
                                                     style->GetFillB(),
                                                     style->GetFillA())));
      break;
    case SymbolStyle::triangle: {
      path.moveTo(x-style->GetSize()/2,y+style->GetSize()/2);
      path.lineTo(x,y-style->GetSize()/2);
      path.lineTo(x+style->GetSize()/2,y+style->GetSize()/2);
      path.lineTo(x-style->GetSize()/2,y+style->GetSize()/2);
      painter->fillPath(path,QBrush(QColor::fromRgbF(style->GetFillR(),
                                                     style->GetFillG(),
                                                     style->GetFillB(),
                                                     style->GetFillA())));
    }
      break;
    }
  }

  void MapPainterQt::DrawPath(const Projection& projection,
                              const MapParameter& parameter,
                              double r,
                              double g,
                              double b,
                              double a,
                              double width,
                              const std::vector<double>& dash,
                              CapStyle startCap,
                              CapStyle endCap,
                              size_t transStart, size_t transEnd)
  {
    QPen pen;

    pen.setColor(QColor::fromRgbF(r,g,b,a));
    pen.setWidthF(width);
    pen.setJoinStyle(Qt::RoundJoin);

    if (startCap==capRound &&
        endCap==capRound &&
        dash.empty()) {
      pen.setCapStyle(Qt::RoundCap);
    }
    else {
      pen.setCapStyle(Qt::FlatCap);
    }

    if (dash.empty()) {
      pen.setStyle(Qt::SolidLine);
    }
    else {
      QVector<qreal> dashes;

      for (size_t i=0; i<dash.size(); i++) {
        dashes << dash[i];
      }

      pen.setDashPattern(dashes);
    }

/*
    painter->setPen(pen);
    size_t last=0;
    bool start=true;
    for (size_t i=0; i<nodes.size(); i++) {
      if (drawNode[i]) {
        if (start) {
          start=false;
        }
        else {
          painter->drawLine(QPointF(nodeX[last],nodeY[last]),QPointF(nodeX[i],nodeY[i]));
        }

        last=i;
      }
    }*/

    QPainterPath p;

    for (size_t i=transStart; i<=transEnd; i++) {
      if (i==transStart) {
        p.moveTo(transBuffer.buffer[i].x,transBuffer.buffer[i].y);
      }
      else {
        p.lineTo(transBuffer.buffer[i].x,transBuffer.buffer[i].y);
      }
    }

    painter->strokePath(p,pen);
/*
    QPolygonF polygon;
    for (size_t i=0; i<nodes.size(); i++) {
      if (drawNode[i]) {
        polygon << QPointF(nodeX[i],nodeY[i]);
      }
    }

    painter->setPen(pen);
    painter->drawPolyline(polygon);*/

    if (dash.empty() &&
        startCap==capRound &&
        endCap!=capRound) {
      painter->setBrush(QBrush(QColor::fromRgbF(r,g,b,a)));

      painter->drawEllipse(QPointF(transBuffer.buffer[transStart].x,
                                   transBuffer.buffer[transStart].y),
                                   width/2,width/2);
    }

    if (dash.empty() &&
      endCap==capRound &&
      startCap!=capRound) {
      painter->setBrush(QBrush(QColor::fromRgbF(r,g,b,a)));

      painter->drawEllipse(QPointF(transBuffer.buffer[transEnd].x,
                                   transBuffer.buffer[transEnd].y),
                                   width/2,width/2);
    }
  }

  void MapPainterQt::DrawArea(const Projection& projection,
                              const MapParameter& parameter,
                              const MapPainter::AreaData& area)
  {
    QPolygonF polygon;

    for (size_t i=area.transStart; i<=area.transEnd; i++) {
      polygon << QPointF(transBuffer.buffer[i].x,transBuffer.buffer[i].y);
    }

    double borderWidth=GetProjectedWidth(projection, area.fillStyle->GetBorderMinPixel(), area.fillStyle->GetBorderWidth());

    if (borderWidth>0.0) {
      QPen pen;

      pen.setColor(QColor::fromRgbF(area.fillStyle->GetBorderR(),
                                    area.fillStyle->GetBorderG(),
                                    area.fillStyle->GetBorderB(),
                                    area.fillStyle->GetBorderA()));
      pen.setWidthF(borderWidth);

      if (area.fillStyle->GetBorderDash().empty()) {
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
      }
      else {
        QVector<qreal> dashes;

        for (size_t i=0; i<area.fillStyle->GetBorderDash().size(); i++) {
          dashes << area.fillStyle->GetBorderDash()[i];
        }

        pen.setDashPattern(dashes);
        pen.setCapStyle(Qt::FlatCap);
      }

      painter->setPen(pen);
    }
    else {
      painter->setPen(Qt::NoPen);
    }

    SetBrush(parameter,
             *area.fillStyle);

    painter->drawPolygon(polygon);
  }

  void MapPainterQt::DrawArea(const FillStyle& style,
                              const MapParameter& parameter,
                              double x,
                              double y,
                              double width,
                              double height)
  {
    painter->fillRect(QRectF(x,y,width,height),
                      QBrush(QColor::fromRgbF(style.GetFillR(),
                                              style.GetFillG(),
                                              style.GetFillB(),
                                              1)));
  }

  void MapPainterQt::SetPen(const LineStyle& style,
                            double lineWidth)
  {
    QPen pen;

    pen.setColor(QColor::fromRgbF(style.GetLineR(),
                                  style.GetLineG(),
                                  style.GetLineB(),
                                  style.GetLineA()));
    pen.setWidthF(lineWidth);

    if (style.GetDash().empty()) {
      pen.setStyle(Qt::SolidLine);
      pen.setCapStyle(Qt::RoundCap);
    }
    else {
      QVector<qreal> dashes;

      for (size_t i=0; i<style.GetDash().size(); i++) {
        dashes << style.GetDash()[i];
      }

      pen.setDashPattern(dashes);
      pen.setCapStyle(Qt::FlatCap);
    }

    painter->setPen(pen);
  }

  void MapPainterQt::SetBrush()
  {
    painter->setBrush(Qt::NoBrush);
  }

  void MapPainterQt::SetBrush(const MapParameter& parameter,
                              const FillStyle& fillStyle)
  {
    if (fillStyle.HasPattern() && HasPattern(parameter,fillStyle)) {
      painter->setBrush(patterns[fillStyle.GetPatternId()-1]);
    }
    else {
      painter->setBrush(QBrush(QColor::fromRgbF(fillStyle.GetFillR(),
                                                fillStyle.GetFillG(),
                                                fillStyle.GetFillB(),
                                                fillStyle.GetFillA())));
    }
  }

  bool MapPainterQt::DrawMap(const StyleConfig& styleConfig,
                             const Projection& projection,
                             const MapParameter& parameter,
                             const MapData& data,
                             QPainter* painter)
  {
    this->painter=painter;

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);

    Draw(styleConfig,
         projection,
         parameter,
         data);

    return true;
  }


  void MapPainterQt::PreparePartitionData(QString partitionOutPath)
  {      

    if(partitionOutPath==pDatasourcePath)
        return;

    this->pDatasourcePath=partitionOutPath;

    QFile file(partitionOutPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray line;
    while(line.startsWith("#") || line.startsWith("\n") || line.isNull())
    line = file.readLine();
    int nodes = QString(line).toInt();
    int cells = QString(file.readLine()).toInt();
    int ways  = QString(file.readLine()).toInt();
    line = file.readLine();
    // Process nodes
    while (!file.atEnd() && !line.startsWith("\n")) {
        QStringList pLine = QString(line).split(' ');
        pNode newNode;
        newNode.no = pLine.at(0).toInt();
        newNode.lon = pLine.at(1).toDouble();
        newNode.lat = pLine.at(2).toDouble();
        newNode.cellNo = pLine.at(3).toDouble();
        pNodes.append(newNode);
        line = file.readLine();
    }
    line = file.readLine();
    // Process ways
    while (!file.atEnd()) {
        QStringList pLine = QString(line).split(' ');
        pWay newWay;
        newWay.no = pLine.at(0).toInt();
        newWay.nodesCount = pLine.at(1).toInt();
        for(int i = 2; i < pLine.size(); i++){
            newWay.nodes.append(pLine.at(i).toInt());
        }
        pWays.append(newWay);
        line = file.readLine();
    }

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
  }

  void MapPainterQt::RenderPartitionResults(const Projection& projection, QPainter* painter)
  {
      this->painter = painter;

      // Display nodes
      foreach(pNode myNode, pNodes)
      {
          painter->setPen(pColors.at(myNode.cellNo));
          painter->setBrush(pColors.at(myNode.cellNo));
          double x, y;
          projection.GeoToPixel(myNode.lon, myNode.lat, x, y);
          painter->drawEllipse(QPointF(x,y),2,2);
      }

      // Display ways
      foreach(pWay myWay, pWays)
      {
          for(int i = 0; i < myWay.nodesCount-1; i++)
          {
              pNode currNode = pNodes.at(myWay.nodes.at(i));
              pNode nextNode = pNodes.at(myWay.nodes.at(i+1));

              QPen pen;

              pen.setColor(pColors.at(currNode.cellNo));
              pen.setWidthF(2);
              pen.setJoinStyle(Qt::RoundJoin);
              pen.setCapStyle(Qt::RoundCap);
              pen.setStyle(Qt::SolidLine);

              painter->setPen(pen);
              double x1, y1, x2, y2;
              projection.GeoToPixel(currNode.lon, currNode.lat, x1, y1);
              projection.GeoToPixel(nextNode.lon, nextNode.lat, x2, y2);
              QPainterPath path(QPointF(x1,y1));
              path.lineTo(x2,y2);
              painter->setPen(pen);
              painter->drawPath(path);
          }
      }
  }
}
