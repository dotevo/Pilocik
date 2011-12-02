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
    void NewRoute();

public slots:
    void RoutingProgressSlot(int progress);
    void RoutingErrorSlot(QString errorMessage);

private:
    osmscout::Routing *routing;
    PiLibocik::Partition::PartitionFile *partitionFile;

    /**
     * @brief This method calculates route for routing edge.
     *
     * @param startPosition - position of beggining of routing edge
     * @param endPosition - position of ending of routing edge
     *
     * @return list of Steps without routing edges
     */
    QList< osmscout::Routing::Step > routingEdgeToRoute(PiLibocik::Position startPosition, PiLibocik::Position endPosition);

};

#endif // ROUTINGMANAGER_H
