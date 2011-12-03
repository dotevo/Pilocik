#ifndef TCLOCKWIDGET_H
#define TCLOCKWIDGET_H

#include <QFrame>
#include "../../pluginmanager.h"
#include "../../pluginwidget.h"

namespace Ui {
    class TClockWidget;
}

class TClockWidget : public PluginWidget
{
    Q_OBJECT
public:
    TClockWidget(QWidget *parent=0);
    ~TClockWidget();
    QString getWidgetName(){ return "SimpleClock";}

private:
    Ui::TClockWidget *ui;

public slots:
    void updateTime();
};

#endif // TCLOCKWIDGET_H
