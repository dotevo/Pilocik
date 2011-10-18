#ifndef GPSRECEIVER_H
#define GPSRECEIVER_H

#ifdef Q_OS_WIN
    #include <windows.h>
#endif
#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QList>

class GPSdata
{
public:
    double lat, lon, alt, speed, angle;
    int tracedSat;
    int active;
    QStringList satelitesPRN;
    QList<int> satelitesSNR;
    QString rawOutput;

    GPSdata();
    void parseBuffer(QStringList* gpsDataBuffer);
};

class GPSreceiver : public QThread
{
    Q_OBJECT

private:
#ifdef Q_OS_WIN
    HANDLE hInput;
#endif
    QStringList output;
    QString path;
    int mode;
    bool contiunue;

    int connectSerialPort();
    bool testSerialPort();
    bool startSimulation();
    bool startRealGPS();

public:
    const static int GPS_MODE = 1;
    const static int SIMULATION_MODE = 2;
    QString serialPort;

    GPSreceiver();
    ~GPSreceiver();
    void setSimPath(QString path);
    void run();
    void disable();
    void setMode(int m);
    QStringList* getOutputBuffer();
    void clearBuffer();

signals:
    void positionUpdate(GPSdata gpsData);
    void statusUpdate(QString status);
};

#endif // GPSRECEIVER_H
