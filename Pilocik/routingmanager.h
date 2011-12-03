#ifndef ROUTINGMANAGER_H
#define ROUTINGMANAGER_H

#include <QThread>

#include "../lib/sosmscout/include/osmscout/Routing.h"
#include "navigationwindow.h"

class RoutingManager : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     *
     */
    explicit RoutingManager();
    /**
     * @brief Destructor.
     *
     */
    ~RoutingManager();
    /**
     * @brief
     *
     */
    void run();
    /**
     * @brief Gets pointer to Routing file object.
     *
     * @return routing object
     */
    osmscout::Routing *getRouting();
    /**
     * @brief Gets pointer to PartitionFile object.
     *
     * @return PartitionFile object
     */
    PiLibocik::Partition::PartitionFile *getPartitionFile();

signals:
    /**
     * @brief Signal emited after routing manager changed route.
     *
     */
    void NewRoute();

public slots:
    /**
     * @brief Slot for routing progress changes for changing progres in widget.
     *
     * @param progress - routing progress to set
     */
    void RoutingProgressSlot(int progress);
    /**
     * @brief Slot for routing error message for showing in widget.
     *
     * @param errorMessage - message to show
     */
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
