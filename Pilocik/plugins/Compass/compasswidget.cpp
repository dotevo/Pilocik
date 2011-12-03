#include "compasswidget.h"
#include "ui_compasswidget.h"
#include <QPainter>

CompassWidget::CompassWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::CompassWidget)
{
    ui->setupUi(this);
    pn=360;
}

CompassWidget::~CompassWidget()
{
    delete ui;
}

void CompassWidget::setAngle(double angle){
    pn=360-angle;
    repaint();
}


void CompassWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPolygon marker;
    QMatrix matrix, matrix2, result;
    marker << QPoint(-10,8) << QPoint(10,8) << QPoint(0,-15);

    matrix.rotate(pn);
    matrix2.translate(30, 30);

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

    painter.setBrush(QBrush(grad));
    painter.setPen(QPen(QColor::fromRgb(255,255,255,255)));
    painter.drawPolygon(marker);
}
