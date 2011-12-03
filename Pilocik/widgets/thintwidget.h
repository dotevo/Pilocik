#ifndef THINTWIDGET_H
#define THINTWIDGET_H

#include "../tmovableframe.h"
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QPainter>
#include <QPointF>
#include <QList>

#include <osmscout/Searching.h>

namespace Ui {
    class THintWidget;
}

class THintWidget : public TMovableFrame
{
    Q_OBJECT

public:

    explicit THintWidget(QWidget *parent = 0);
    ~THintWidget();

    void updateDistance(double distance);
    void setFinishRoute();
    void setLeaveRoute();

    void setIntersection(osmscout::Searching::Intersection intersection);

    QPointF calculateWay(double px, double py);

    //void drawIntersection(QPainter *painter);

    osmscout::Searching::Intersection *intersection;

protected:
       void paintEvent(QPaintEvent *e);

private:
    Ui::THintWidget *ui;

    QList<QPointF> *ways;

    QString mUnit;
    QString kmUnit;

    QString finishRoute;
    QString leaveRoute;

    double myPosX, myPosY, crossX, crossY, wayX, wayY;

    double distance;



    //osmscout::Searching::Intersection *intersection;


};

#endif // THINTWIDGET_H
