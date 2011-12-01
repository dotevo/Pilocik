#ifndef ROUTINGMANAGER_H
#define ROUTINGMANAGER_H

#include <QThread>

#include "../lib/sosmscout/include/osmscout/Routing.h"
#include "navigationwindow.h"

class RoutingManager : public QThread
{
    Q_OBJECT
public:
    explicit RoutingManager();
    void run();

    osmscout::Routing *getRouting();

signals:

public slots:

private:
    osmscout::Routing *routing;

};

#endif // ROUTINGMANAGER_H
