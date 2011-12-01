#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H

#include <QVector>

#include "qfullscreenframe.h"
#include "pointselectionwindow.h"
#include "routingmanager.h"
#include "../lib/PiLibocik/include/pilibocik/position.h"

namespace Ui {
    class RouteWindow;
}

class NavigationWindow;

class RouteWindow : public QFullScreenFrame{
    Q_OBJECT
public:
    explicit RouteWindow(NavigationWindow *parent = 0);
    ~RouteWindow();

    PiLibocik::Position getFrom();
    PiLibocik::Position getTo();
    QList< PiLibocik::Position > getThrough();

public slots:
    void startSet(double lon, double lat, QString name);
    void targetSet(double lon, double lat, QString name);
    void addStop(double lon, double lat, QString name);

private slots:
    void on_toButton_clicked();
    void on_routeBackButton_clicked();
    void pswClosed();
    void on_okButton_clicked();

    void on_clearThroughButton_clicked();

signals:
    void closed();

private:
    Ui::RouteWindow *ui;
    PointSelectionWindow* psw;
    RoutingManager *routingManager;
    QPair< QString, PiLibocik::Position > from;
    QPair< QString, PiLibocik::Position > to;
    QList< QPair< QString, PiLibocik::Position > > through;

    void initThroughList();
    void changeEvent(QEvent *);
};

#endif // ROUTEWINDOW_H
