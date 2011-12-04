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
#include <QBuffer>
#include <QSharedMemory>
#ifdef Q_OS_WINCE
    #include <windows.h>
#endif

#include "gpsdata.h"



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
    QString path, serialPort;
    QFile file;
    QSharedMemory sharedMem;
    int mode;
    bool contiunue;

    QList<osmscout::Routing::Step> simulationRoute;

    int connectSerialPort();
    bool testSerialPort();
    bool startSimulation();
    bool startPcSimulation();
    bool startRealGPS();

public:
    const static int GPS_MODE = 1;          //!< Representation of gps mode value for mode selector
    const static int SIMULATION_MODE = 2;   //!< Representation of simulation mode value for mode selector
    const static int PC_SIMULATION_MODE = 3;//!< Representation of PC simulation mode via shared memory for mode selector

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

    /**
      @brief Parsing file with gps data to gets route.
      @return True if file is parsed to end.
      */
    bool parseFile();

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
