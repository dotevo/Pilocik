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
    const static int GPS_MODE = 1;
    const static int SIMULATION_MODE = 2;
    QString serialPort;

    GPSreceiver();
    ~GPSreceiver();
    void setSimPath(QString path);
    void disable();
    void setMode(int m);
    void clearBuffer();

protected:
    void run();

signals:
    void positionUpdate(GPSdata gpsData);
    void statusUpdate(QString status);
    void simStatusUpdate(QString status);
};

#endif // GPSRECEIVER_H
