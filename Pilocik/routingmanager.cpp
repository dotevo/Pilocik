#include "routingmanager.h"

#include "navigationwindow.h"
#include "routewindow.h"
#include "widgets/troutingprogresswidget.h"
#include "twidgetmanager.h"
#include "../lib/PiLibocik/include/pilibocik/position.h"

#include <QDebug>

RoutingManager::RoutingManager()
{
    routing = new osmscout::Routing();
}

void RoutingManager::run()
{
    qDebug() << "cokolwiek";
    QList< osmscout::Routing::Step > routeList;
    QVector< osmscout::Routing::Step > routeVector;

    if(NavigationWindow::main->routeWin->getThrough().isEmpty()) {
        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), NavigationWindow::main->routeWin->getTo());
        qDebug() << "cokolwiek2";
    } else {
        QList< PiLibocik::Position > through = NavigationWindow::main->routeWin->getThrough();
        QListIterator< PiLibocik::Position > throughIterator(through);

        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), throughIterator.next());

        routeList.append(routing->positionsToSteps(through));
    }

    QListIterator< osmscout::Routing::Step > routeIterator(routeList);
    while(routeIterator.hasNext()) {
        osmscout::Routing::Step step = routeIterator.next();
        routeVector.push_back(step);
    }

    NavigationWindow::main->mapRenderer->setRoute(routeVector);

    qDebug() << "cokolwiek4";
}

osmscout::Routing *RoutingManager::getRouting()
{
    return routing;
}
