#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H

#include <QVector>

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
    void targetSet(double lon, double lat, QString name);

signals:
    void windowClosed();

private:
    Ui::RouteWindow *ui;
    PointSelectionWindow* psw;

    void changeEvent(QEvent *);
};

#endif // ROUTEWINDOW_H
