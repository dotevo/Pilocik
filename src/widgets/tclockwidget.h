#ifndef TCLOCKWIDGET_H
#define TCLOCKWIDGET_H

#include "../twidget.h"

namespace Ui {
    class TClockWidget;
}

class TClockWidget : public TWidget
{
    Q_OBJECT

public:
    explicit TClockWidget(QWidget *parent = 0);
    ~TClockWidget();

private:
    Ui::TClockWidget *ui;
};

#endif // TCLOCKWIDGET_H
