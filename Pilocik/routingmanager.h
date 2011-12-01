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
    ~RoutingManager();
    void run();

    osmscout::Routing *getRouting();
    PiLibocik::Partition::PartitionFile *getPartitionFile();

signals:
    void RoutingProgressSignal(int progress);

public slots:
    void RoutingProgressSlot(int progress);

private:
    osmscout::Routing *routing;
    PiLibocik::Partition::PartitionFile *partitionFile;

};

#endif // ROUTINGMANAGER_H
