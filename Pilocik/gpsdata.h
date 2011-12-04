#ifndef GPSDATA_H
#define GPSDATA_H

#include <QStringList>

#include <osmscout/Routing.h>

class GPSdata
{
public:

    double  lat,                //!< Latitude coordinate
            lon,                //!< Longitude coordinate
            alt,                //!< Height over sea level
            speed,              //!< Current moving speed in km/h
            angle;              //!< Current moving directory in degrees
    int tracedSat;              //!< Number of currently
    bool active;                //!< When position can be obtained from gps it's true
    QStringList satelitesPRN;   //!< List with names(PRNs) of scanned satelites
    QList<int> satelitesSNR;    //!< Scanned satelites Signal To Noise ratio, represents strength of signal
    QString rawOutput;          //!< Unparsed raw data from GPS device

    GPSdata();

    /**
     * @brief GPS data parser, as input gets data buffer from gps device,
     *        calculates gps information and distribute it to proper variables.
     *
     * @param gpsDataBuffer List of atomic NMEA GPS output information.
     */
    void parseBuffer(QStringList* gpsDataBuffer);

    /**
      @brief It gets part of route from buffer.
      */
    osmscout::Routing::Step getRouteFromBuffer(QStringList* gpsDataBuffer);
};


#endif // GPSDATA_H
