#include "routingmanager.h"

#include "navigationwindow.h"
#include "routewindow.h"
#include "widgets/troutingprogresswidget.h"
#include "twidgetmanager.h"
#include "../lib/PiLibocik/include/pilibocik/position.h"

#include <QDebug>

RoutingManager::RoutingManager()
{
    QString dbPath("files");
    partitionFile = new PiLibocik::Partition::PartitionFile(dbPath, "car", QIODevice::ReadOnly, 1);
    routing = new osmscout::Routing(partitionFile);

    connect(routing, SIGNAL(RoutingProgress(int)), this, SLOT(RoutingProgressSlot(int)));
}

RoutingManager::~RoutingManager()
{
    delete routing;
    delete partitionFile;
}

void RoutingManager::run()
{
    QList< osmscout::Routing::Step > routeList;
    QVector< osmscout::Routing::Step > routeVector;

    if(NavigationWindow::main->routeWin->getThrough().isEmpty()) {
        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), NavigationWindow::main->routeWin->getTo());
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
}

osmscout::Routing *RoutingManager::getRouting()
{
    return routing;
}

PiLibocik::Partition::PartitionFile *RoutingManager::getPartitionFile()
{
    return partitionFile;
}

void RoutingManager::RoutingProgressSlot(int progress)
{
    ((TRoutingProgressWidget *) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->setProgress(progress);
}
