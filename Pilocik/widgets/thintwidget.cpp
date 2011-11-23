#include "thintwidget.h"
#include "ui_thintwidget.h"

THintWidget::THintWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::THintWidget)
{
    ui->setupUi(this);

    finishRoute = "You are at the destination! Good job dude!";
    leaveRoute = "You left the route!";
}

THintWidget::~THintWidget()
{

}

void THintWidget::updateDistance(double distance)
{
    ui->label->setText(QString::number(distance));
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

    wayX = intersection.way.x();
    wayY = intersection.way.y();
}

void THintWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setRenderHint(painter.Antialiasing);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(10);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);

    painter.setPen(pen);

    painter.drawLine(myPosX, myPosY, crossX, crossY);
    painter.drawLine(crossX, crossY, wayX, wayY);

    /*
    pen.setColor(Qt::white);
    for (int i = 0; i < intersection->ways.size(); i++) {
        painter.drawLine(intersection->cross, intersection->ways[i]);
    }
    */
}


