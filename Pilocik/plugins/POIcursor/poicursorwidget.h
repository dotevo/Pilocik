#ifndef POICURSORWIDGET_H
#define POICURSORWIDGET_H

#include <QFrame>
#include "../../pluginwidget.h"


namespace Ui {
    class POIcursorWidget;
}

class POIcursorWidget : public PluginWidget
{
    Q_OBJECT
public:
    POIcursorWidget(QWidget *parent=0);
    ~POIcursorWidget();
    QString getWidgetName(){ return "POIcursorWidget";}
    void setAngle(double angle);
    void paintEvent(QPaintEvent *e);
private:
    Ui::POIcursorWidget *ui;
    double pn;
};


#endif // POICURSORWIDGET_H
