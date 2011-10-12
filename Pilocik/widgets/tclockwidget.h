#ifndef TCLOCKWIDGET_H
#define TCLOCKWIDGET_H

#include "../tmovableframe.h"

namespace Ui {
    class TClockWidget;
}

class TClockWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TClockWidget(QGraphicsScene *parent);
    ~TClockWidget();

private:
    Ui::TClockWidget *ui;

public slots:
    void updateTime();
};

#endif // TCLOCKWIDGET_H
