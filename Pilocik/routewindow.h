#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H

#include "qfullscreenframe.h"
#include "pointselectionwindow.h"

namespace Ui {
    class RouteWindow;
}

class NavigationWindow;

class RouteWindow : public QFullScreenFrame{
    Q_OBJECT
public:
    explicit RouteWindow(NavigationWindow *parent = 0);
    ~RouteWindow();

private slots:
    void on_toButton_clicked();
    void on_routeBackButton_clicked();
    void pswClosed();

private:
    Ui::RouteWindow *ui;
    PointSelectionWindow* psw;
};

#endif // ROUTEWINDOW_H
