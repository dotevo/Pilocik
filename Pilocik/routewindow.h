#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H

#include <QFrame>

namespace Ui {
    class RouteWindow;
}

class RouteWindow : public QFrame
{
    Q_OBJECT

public:
    explicit RouteWindow(QWidget *parent = 0);
    ~RouteWindow();

private:
    Ui::RouteWindow *ui;
};

#endif // ROUTEWINDOW_H
