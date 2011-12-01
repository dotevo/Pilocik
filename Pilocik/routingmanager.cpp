#include "routingmanager.h"

#include "navigationwindow.h"
#include "routewindow.h"
#include "widgets/troutingprogresswidget.h"
#include "twidgetmanager.h"
#include "../lib/PiLibocik/include/pilibocik/position.h"

#include <QDebug>

RoutingManager * RoutingManager::instance = 0;

RoutingManager::RoutingManager()
{
    QString dbPath("");
    partitionFile = new PiLibocik::Partition::PartitionFile(dbPath, "car", QIODevice::ReadOnly, 1);
    //routing = new osmscout::Routing(partitionFile);
    routing = new osmscout::Routing();

    connect(routing, SIGNAL(RoutingProgress(int)), this, SLOT(RoutingProgressSlot(int)));
}

RoutingManager::~RoutingManager()
{
    delete routing;
    delete partitionFile;
}

RoutingManager* RoutingManager::getInstance()
{
    if (instance == 0)
        instance = new RoutingManager();
    return instance;
}

void RoutingManager::run()
{
    QList< osmscout::Routing::Step > routeList;
    //QVector< osmscout::Routing::Step > routeVector;

    if(NavigationWindow::main->routeWin->getThrough().isEmpty()) {
        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), NavigationWindow::main->routeWin->getTo());
    } else {
        QList< PiLibocik::Position > through = NavigationWindow::main->routeWin->getThrough();
        QListIterator< PiLibocik::Position > throughIterator(through);

        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), throughIterator.next());

        through.append(NavigationWindow::main->routeWin->getTo());
        routeList.append(routing->positionsToSteps(through));
    }

    /*
    QListIterator< osmscout::Routing::Step > routeIterator(routeList);
    while(routeIterator.hasNext()) {
        osmscout::Routing::Step step = routeIterator.next();
        routeVector.push_back(step);
    }
    */

<<<<<<< HEAD
    NavigationWindow::main->mapRenderer->setRoute(routeList);
=======
    NavigationWindow::main->mapRenderer->setRoute((QVector< osmscout::Routing::Step >) routeVector);
    emit NewRoute();
>>>>>>> 1f5e92dd971e9eb3a8ac933b6aad4e1d60abdf1e
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
