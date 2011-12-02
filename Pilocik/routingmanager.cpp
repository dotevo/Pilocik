#include "routingmanager.h"

#include "navigationwindow.h"
#include "routewindow.h"
#include "widgets/troutingprogresswidget.h"
#include "widgets/terrorwidget.h"
#include "twidgetmanager.h"
#include "../lib/PiLibocik/include/pilibocik/position.h"

#include <QDebug>

RoutingManager::RoutingManager()
{
    QString dbPath("test01");
    partitionFile = new PiLibocik::Partition::PartitionFile(dbPath, "car", QIODevice::ReadOnly, 1);
    routing = new osmscout::Routing(partitionFile);

    connect(routing, SIGNAL(RoutingProgress(int)), this, SLOT(RoutingProgressSlot(int)));
    connect(routing, SIGNAL(Error(QString)), this, SLOT(RoutingErrorSlot(QString)));
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

    // first
    if(NavigationWindow::main->routeWin->getThrough().isEmpty()) {
        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), NavigationWindow::main->routeWin->getTo());
    } else {
        QList< PiLibocik::Position > through = NavigationWindow::main->routeWin->getThrough();
        QListIterator< PiLibocik::Position > throughIterator(through);

        routeList = routing->CalculateRoute(NavigationWindow::main->routeWin->getFrom(), throughIterator.next());

        through.append(NavigationWindow::main->routeWin->getTo());
        routeList.append(routing->positionsToSteps(through));
    }

    QListIterator< osmscout::Routing::Step > routeIterator(routeList);
    osmscout::Routing::Step step = routeIterator.next();
    PiLibocik::Position prevPosition(step.lon, step.lat);
    while(routeIterator.hasNext()) {
        step = routeIterator.next();
        if(step.routing) {
            NavigationWindow::main->mapRenderer->setRoute((QVector< osmscout::Routing::Step >) routeVector);
            emit NewRoute();

            QList< osmscout::Routing::Step > routingEdgeRoute = routingEdgeToRoute(prevPosition, PiLibocik::Position(step.lon, step.lat));

            QListIterator< osmscout::Routing::Step > routingEdgeIterator(routingEdgeRoute);
            while(routingEdgeIterator.hasNext()) {
                step = routingEdgeIterator.next();

                prevPosition.setLon(step.lon);
                prevPosition.setLat(step.lat);
                routeVector.push_back(step);
            }
        } else {
            prevPosition.setLon(step.lon);
            prevPosition.setLat(step.lat);
            routeVector.push_back(step);
        }
    }

    NavigationWindow::main->mapRenderer->setRoute((QVector< osmscout::Routing::Step >) routeVector);
    emit NewRoute();
}

QList< osmscout::Routing::Step > RoutingManager::routingEdgeToRoute(PiLibocik::Position startPosition, PiLibocik::Position endPosition)
{
    QList< osmscout::Routing::Step > routeList;
    QList< osmscout::Routing::Step > result;

    routeList = routing->CalculateRoute(startPosition, endPosition);

    QListIterator< osmscout::Routing::Step > routeIterator(routeList);
    osmscout::Routing::Step step = routeIterator.next();
    PiLibocik::Position prevPosition(step.lon, step.lat);
    while(routeIterator.hasNext()) {
        step = routeIterator.next();
        if(step.routing) {
            QList< osmscout::Routing::Step > routingEdgeRoute = routingEdgeToRoute(prevPosition, PiLibocik::Position(step.lon, step.lat));

            QListIterator< osmscout::Routing::Step > routingEdgeIterator(routingEdgeRoute);
            while(routingEdgeIterator.hasNext()) {
                step = routingEdgeIterator.next();
                result.push_back(step);
            }
        } else {
            prevPosition.setLon(step.lon);
            prevPosition.setLat(step.lat);
            result.push_back(step);
        }
    }

    return result;
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

void RoutingManager::RoutingErrorSlot(QString errorMessage)
{
    ((TRoutingProgressWidget *) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->stopCalculating();
    ((TErrorWidget *) TWidgetManager::getInstance()->getWidget("ErrorMessage"))->showMessage(errorMessage);
}
