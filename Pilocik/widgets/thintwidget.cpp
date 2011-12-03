#include "thintwidget.h"
#include "ui_thintwidget.h"
#include <QDebug>
#include <math.h>
#include <qmath.h>

THintWidget::THintWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::THintWidget)
{
    ui->setupUi(this);

    //finishRoute = "You are at the destination! Good job dude!";
    finishRoute = "FINISH";
    leaveRoute = "You left the route!";

    intersection = new osmscout::Searching::Intersection();

    distance = -1;

}

THintWidget::~THintWidget()
{

}

void THintWidget::updateDistance(double distance)
{
    this->distance = distance;
    if (distance < 1000)
        ui->label->setText(QString::number(distance) + "m");
    else {
        ui->label->setText(QString::number(distance/1000, 'f', 2) + "km");
    }
}

void THintWidget::setFinishRoute()
{
    ui->label->setText(finishRoute);
}

void THintWidget::setLeaveRoute()
{
    ui->label->setText(leaveRoute);
}

void THintWidget::setIntersection(osmscout::Searching::Intersection intersection)
{
    this->intersection = &intersection;

    myPosX = intersection.myPos.x();
    myPosY = intersection.myPos.y();

    crossX = intersection.cross.x();
    crossY = intersection.cross.y();

    QPointF way = calculateWay(intersection.way.x(), intersection.way.y());

    wayX = way.x();
    wayY = way.y();

    //wayX = intersection.way.x();
    //wayY = intersection.way.y();
}

QPointF THintWidget::calculateWay(double px, double py)
{
    /*
    double L = 30;
    double cx = crossX;
    double cy = crossY;

    double a = (cy - py) / (cx - px);
    double b = py - (a * px);

    //double delta = pow(2 * a * b - 2 * cy - 2 * cx, 2) - 4 * (1 + a) * (pow(cx, 2) + pow(b, 2) - 2 * b * cy + pow(cy, 2) - pow(L, 2));
    double da = 1 + (a * a);
    //double db = (2 * a * b) - (2 * cx) - (2 * a * cy);
    double db = 2 * a * (pow(b - cy, 2)) - 2 * cx;
    //double dc = (cx * cx) + (cy * cy) - (2 * cy * b) + (b * b) - (L * L);
    double dc = cx * cx + pow(b - cy, 2) - (L * L);

    double delta = (db * db) - (4 * da * dc);
    qDebug() << "A: " << da << " B: " << db << " C: " << dc << " DELTA: " << delta;

    double x1 = (-db - sqrt(delta)) / (2*da);
    double x2 = (-db + sqrt(delta)) / (2*da);

    qDebug() << "Results: " << x1 << " " << x2;

    if (x1 > 0) {
        double y1 = a * x1 + b;
        return QPointF(x1, y1);
    } else if (x2 > 0) {
        double y2 = a * x2 + b;
        return QPointF(x2, y2);
    }
    */

    double cx = crossX;
    double cy = crossY;

    double a = (cy - py) / (cx - px);
    double b = py - (a * px);

    double d = 100;

    double alfa = atan(a);

    double dx = d - qAbs(px - cx);
    double dy = d - qAbs(py - cy);

//    qDebug() << "a: " << a << " ALFA: " << alfa << " SIN: " << sin(alfa) << " COS: " << cos(alfa);
//    qDebug() << "px = " << px << " py = " << py;
//    qDebug() << "D: " << dx << " " << dy;

    double kx = qAbs(cos(alfa) * d);
    double ky = qAbs(sin(alfa) * d);

//    qDebug() << "K: " << kx << " " << ky;

    if (px < cx)
        kx = -kx;

    if (py < cy)
        ky = -ky;

    QPointF p(px + kx, py + ky);

//    qDebug() << "px = " << p.x() << " py = " << p.y();
//    qDebug() << "Distance: " << sqrt(pow(cx - p.x(), 2) + pow(cy - p.y(), 2));

    return p;
}

void THintWidget::paintEvent(QPaintEvent *e)
{
    // it should be mod 2 == 0
    int lineW = 10;
    QPainter painter(this);

    if (distance < 100) {
        QLinearGradient gradient(30, 10, 30, 30);
        gradient.setColorAt(1, QColor(255, 7*16 + 15, 0, 100 - distance));
        gradient.setColorAt(0, QColor(255, 7*16 + 15, 0, 75 - distance * 3/4));

        QBrush br(gradient);
        painter.setBackground(br);
        painter.fillRect(0, 0, 100, 100, br);
    }

    painter.setRenderHint(painter.Antialiasing);

    QPen pen;
    pen.setColor(QColor("#763b3b"));
    pen.setWidth(lineW);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);

    /*
    for (int i = 0; i < intersection->numOfWays; i++) {
        painter.drawLine(intersection->cross, intersection->ways[i]);
    }
*/
    //painter.drawLine(myPosX, myPosY, crossX, crossY);
    painter.drawLine(myPosX, 100, crossX, crossY);

    painter.drawLine(crossX, crossY, wayX, wayY);

    //qDebug() << wayX << " " << wayY << " " << crossX << " " << crossY;

}


