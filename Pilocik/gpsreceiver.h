#ifndef GPSRECEIVER_H
#define GPSRECEIVER_H

#include <QCoreApplication>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QList>
#include <QFile>
#include <QTimer>
#ifdef Q_OS_WINCE_STD
    #include <windows.h>
#endif


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
};



/**
 * @brief Provides GPS device and Simulation support.
 *
 * GPSreceiver is singleton. It's based on separate thread for data reading.
 * Before starting thread there needs to be made some configuration.
 * First of all mode needs to be selected using setMode() method.
 * If simulation mode choosed, simulation path needs to be specified using setSimPath().
 * After configuration call start() method to start receiving data.
 *
 * @class GPSreceiver gpsreceivier.h "Pilocik/gpsreceivier.h"
 */
class GPSreceiver : public QThread
{
    Q_OBJECT

private:
#ifdef Q_OS_WINCE_STD
    HANDLE hInput;
#endif
    QStringList output;
    QString path;
    QFile file;
    int mode;
    bool contiunue;

    int connectSerialPort();
    bool testSerialPort();
    bool startSimulation();
    bool startRealGPS();

public:
    const static int GPS_MODE = 1;          //!< Representation of gps mode value for mode selector
    const static int SIMULATION_MODE = 2;   //!< Representation of simulation mode value for mode selector

    GPSreceiver();
    ~GPSreceiver();

    /**
     * @brief Set path of simulation file.
     *
     * @param path Path of simulation file.
     */
    void setSimPath(QString path);


    /**
     * @brief Disable gps data receiving from simulation and gps device.
     *
     */
    void disable();


    /**
     * @brief Set working mode.
     *
     * @param m 1:Gps device mode
     *          2:Simulation mode
     *
     * Working mode needs to be set to 1 or 2 before calling start() method to
     * enable GPS data receiving.
     */
    void setMode(int m);


    /**
     * @brief Clears buffer responsible for receiving GPS data.
     *
     * Buffer should be cleaned after each read from it to ensure best performance.
     */
    void clearBuffer();

protected:
    void run();

signals:

    /**
     * @brief Signal emited after receiving new portion of GPS data.
     *
     * @param gpsData Parsed GPS data
     */
    void positionUpdate(GPSdata gpsData);


    /**
     * @brief Signal emited on real gps status change.
     *
     * @param status Text status of gps device.
     */
    void statusUpdate(QString status);


    /**
     * @brief Singal emited on simulation status change.
     *
     * @param status Text status of simulation.
     */
    void simStatusUpdate(QString status);
};

#endif // GPSRECEIVER_H
