#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QFrame>
#include <QPaintEvent>
#include "../../pluginmanager.h"
#include "../../pluginwidget.h"


namespace Ui {
    class CompassWidget;
}

class CompassWidget : public PluginWidget
{
    Q_OBJECT
public:
    CompassWidget(QWidget *parent=0);
    ~CompassWidget();
    QString getWidgetName(){ return "Compass";}
    void setAngle(double angle);
    void paintEvent(QPaintEvent *e);
private:
    Ui::CompassWidget *ui;
    double pn;
};


#endif // COMPASSWIDGET_H
