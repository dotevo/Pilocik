#include "gpsdata.h"
#include "navigationwindow.h"
#include "routewindow.h"
#include "pilibocik/partition/node.h"
#include <cstdlib>

#include <QDebug>

/**
 * @brief GPS data parser and it's container.
 *
 * @class GPSdata gpsreceiver.h "Pilocik/gpsreceiver.h"
 */
GPSdata::GPSdata()
{
    lat=lon=alt=speed=angle=0;
    tracedSat=0;
    active=false;
    satelitesPRN.clear();
    satelitesSNR.clear();
    rawOutput.clear();
}

void GPSdata::parseBuffer(QStringList* gpsDataBuffer)
{
    QStringList buffer = *gpsDataBuffer;
    QStringList GSV, RMCa, GGAa;

    rawOutput = buffer.join("");

    foreach(QString sentence, buffer)
    {
        if(sentence.contains("$GPGGA"))
            GGAa = sentence.split(',');
        if(sentence.contains("$GPRMC"))
            RMCa = sentence.split(',');
        if(sentence.contains("$GPGSV"))
            GSV.append(sentence);
    }

    if (!RMCa.isEmpty())
    {
        div_t cLat, cLon;
        active = true;
        lat = RMCa.at(3).toDouble();
        int dir = (RMCa.at(4).compare("N")) ? -1 : 1;
        cLat = div(lat,100);
        lat=cLat.quot+(cLat.rem+(lat-(int)lat))/60;

        lon = RMCa.at(5).toDouble();
        dir = (RMCa.at(6).compare("E")) ? -1 : 1;
        cLon = div(lon,100);
        lon = cLon.quot+(cLon.rem+(lon-(int)lon))/60;
        speed = RMCa.at(7).toDouble()*1.852;
        angle = RMCa.at(8).toDouble();
    }

    if (!GGAa.isEmpty())
    {
        tracedSat = GGAa.at(7).toInt();
        alt = GGAa.at(9).toDouble();
    }

    while(!GSV.isEmpty())
    {
        QStringList sentence = GSV.last().split(',');
        int i = 4;
        while(!sentence.at(i-1).contains("*"))
        {
            satelitesPRN.append(sentence.at(i));
            satelitesSNR.append(int(sentence.at(i+3).split("*").at(0).toInt()*1.66));
            i = i + 4;
        }
        if(sentence.at(2) == "1")
        {
            break;
        }
        GSV.removeLast();
    }
}

void GPSdata::getRouteFromBuffer(QStringList *gpsDataBuffer)
{
    QStringList buffer = *gpsDataBuffer;
    QStringList RMCa;

    foreach(QString sentence, buffer)
    {
        if(sentence.contains("$GPRMC"))
            RMCa = sentence.split(',');
    }

    if (!RMCa.isEmpty())
    {
        div_t cLat, cLon;
        active = true;
        lat = RMCa.at(3).toDouble();
        int dir = (RMCa.at(4).compare("N")) ? -1 : 1;
        cLat = div(lat,100);
        lat=cLat.quot+(cLat.rem+(lat-(int)lat))/60;

        lon = RMCa.at(5).toDouble();
        dir = (RMCa.at(6).compare("E")) ? -1 : 1;
        cLon = div(lon,100);
        lon = cLon.quot+(cLon.rem+(lon-(int)lon))/60;
        speed = RMCa.at(7).toDouble()*1.852;
        angle = RMCa.at(8).toDouble();

        osmscout::Routing::Step step;
        step.lat = lat;
        step.lon = lon;

        PiLibocik::Partition::Node node = NavigationWindow::main->routeWin->routingManager->getPartitionFile()->getNearestNode(PiLibocik::Position(step.lon, step.lat));
        qDebug() << "Node: " << node.getId();

            route.append(step);
            qDebug() << "Dodalem node!";

    }
}

QList<osmscout::Routing::Step> GPSdata::getRoute()
{
    return route;
}
