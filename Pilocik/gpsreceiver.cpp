#include "gpsreceiver.h"
#include <cstdlib>
#include "navigationwindow.h"
#include "twidgetmanager.h"

#include <QDebug>
#include "routewindow.h"
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/way.h>
#include "osmscout/Routing.h"
#include <QList>

GPSreceiver::GPSreceiver() : sharedMem("GPSDATA")
{
    setTerminationEnabled(true);
}

GPSreceiver::~GPSreceiver()
{
    disable();
    #ifdef Q_OS_WINCE_STD
    CloseHandle(hInput);
    #endif
}

void GPSreceiver::setSimPath(QString path)
{
    this->path = path;
}

bool GPSreceiver::startSimulation()
{
    parseFile();

    NavigationWindow::main->mapRenderer->setRoute(simulationRoute);
    NavigationWindow::main->mapRenderer->setRouting(true);
    TWidgetManager::getInstance()->setRouting(true);

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit simStatusUpdate("Unable to open specified file.");
        return false;
    }

    emit simStatusUpdate("Simulation started.");
    contiunue = true;

    while (contiunue && !file.atEnd()) {
        QByteArray line = file.readLine();
        output.append(line);
        if(line.contains("$GPRMC"))
        {
            GPSdata gps_data = GPSdata();
            gps_data.parseBuffer(&output);
            qRegisterMetaType<GPSdata>("GPSdata");
            emit positionUpdate(gps_data);
            msleep(1000);
        }
    }
}

bool GPSreceiver::startPcSimulation()
{
#ifndef Q_OS_WINCE
    emit simStatusUpdate("Simulation started.");
    contiunue = true;

    while (contiunue) {
        if (!sharedMem.attach())
        {
            //If an attempt of reading from the shared memory before data is written
            contiunue;
        }

        QBuffer buffer;
        QDataStream in(&buffer);
        QString text;

        sharedMem.lock();
        buffer.setData((char*)sharedMem.constData(), sharedMem.size());
        buffer.open(QBuffer::ReadOnly);
        in >> text;
        sharedMem.unlock();

        output.append(text.split('\n'));

        // As this is the last process attached to the shared memory segment
        // the shared memory segment is released, destroying its contents
        sharedMem.detach();

        GPSdata gps_data = GPSdata();
        gps_data.parseBuffer(&output);
        qRegisterMetaType<GPSdata>("GPSdata");
        emit positionUpdate(gps_data);
        msleep(1000);
    }
#endif
	return true;
}

int GPSreceiver::connectSerialPort()
{
#ifdef Q_OS_WINCE_STD
    LPCWSTR serialPorts[] = {L"COM1:", L"COM2:", L"COM3:", L"COM4:", L"COM5:", L"COM6:", L"COM7:", L"COM8:", L"COM9:"};
    int serialPortIndex = 0;
    bool connected;
    emit statusUpdate("Connecting...");

    do
    {
        emit statusUpdate("Connecting... (COM"+QString::number(serialPortIndex+1)+")");
        hInput = CreateFile (serialPorts[serialPortIndex],       // Pointer to the name of the port | serialPorts[serialPortIndex]
                         GENERIC_READ, // Access (read-write) mode
                         0,            // Share mode
                         NULL,         // Pointer to the security attribute
                         OPEN_EXISTING,// How to open the serial port
                         0,            // Port attributes
                         NULL);        // Handle to port with attribute
                                       // to copy

        DCB PortDCB;

        // Initialize the DCBlength member.
        PortDCB.DCBlength = sizeof (DCB);

        // Get the default port setting information.
        GetCommState (hInput, &PortDCB);

        // Change the DCB structure settings.
        PortDCB.BaudRate = 4800;				// Current baud
        PortDCB.fBinary = TRUE;               // Binary mode; no EOF check
        PortDCB.fParity = TRUE;               // Enable parity checking
        PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control
        PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control
        PortDCB.fDtrControl = DTR_CONTROL_ENABLE;
                                              // DTR flow control type
        PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity
        PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx
        PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control
        PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control
        PortDCB.fErrorChar = FALSE;           // Disable error replacement
        PortDCB.fNull = FALSE;                // Disable null stripping
        PortDCB.fRtsControl = RTS_CONTROL_ENABLE;
                                              // RTS flow control
        PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on
                                              // error
        PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8
        PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
        PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2

        if (!SetCommState (hInput, &PortDCB))
        {
            connected = false;
            hInput = NULL;
        }
        else
        {
            connected = testSerialPort();
        }
        serialPortIndex++;
    }while(!connected && serialPortIndex<9);

    return connected ? serialPortIndex : 0;
#endif
    return false;
}

bool GPSreceiver::testSerialPort()
{
#ifdef Q_OS_WINCE_STD
    QString test;
    int i = 0;
    while(!test.contains("$") && i++<25)
    {
        BYTE Byte;
        DWORD dwBytesTransferred;
        ReadFile (hInput,               // Port handle
                        &Byte,              // Pointer to data to read
                        1,                  // Number of bytes to read
                        &dwBytesTransferred,// Pointer to number of bytes
                                            // read
                        NULL                // Must be NULL for Windows CE
        );
        test.append(Byte);
    }
    return test.contains("$");
#endif
    return false;
}

bool GPSreceiver::startRealGPS()
{
    #ifdef Q_OS_WINCE_STD
    int port = connectSerialPort();
    serialPort = "COM" + QString::number(port);
    emit statusUpdate("Connected to "+serialPort);
    if(port == 0)
    {
        emit statusUpdate("Connection failed. Unable to find GPS device.");
        return false;
    }

    QString lBuffer;

    contiunue = true;

    while( contiunue )
    {
        if(!lBuffer.startsWith('$'))
        {
            lBuffer.clear();
        }
        BYTE Byte;
        DWORD dwBytesTransferred;
        ReadFile (hInput,               // Port handle
                        &Byte,              // Pointer to data to read
                        1,                  // Number of bytes to read
                        &dwBytesTransferred,// Pointer to number of bytes
                                            // read
                        NULL                // Must be NULL for Windows CE
        );
        lBuffer.append(Byte);
        if(lBuffer.endsWith('\n')&&lBuffer.startsWith('$'))
        {
            output.append(lBuffer);
            if(lBuffer.contains("RMC"))
            {
                GPSdata gps_data = GPSdata();
                gps_data.parseBuffer(&output);
                qRegisterMetaType<GPSdata>("GPSdata");
                emit positionUpdate(gps_data);
            }
            lBuffer.clear();
        }
    }
#endif
    return false;
}

void GPSreceiver::run()
{
    switch(mode){
    case 0:
        return;
    case 1:
        startRealGPS();
        break;
    case 2:
        startSimulation();
        break;
    case 3:
        startPcSimulation();
        break;
    }
}

void GPSreceiver::clearBuffer()
{
    output.clear();
}

bool GPSreceiver::parseFile()
{
    QFile file(path);
    QStringList outputData;
    GPSdata gps_data = GPSdata();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit simStatusUpdate("Unable to open specified file.");
        return false;
    }

    int i = 0;
    QList<int> ids;
    double dist = 99999;
    PiLibocik::Partition::Node node;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        outputData.append(line);
        int lastCross = -1;
        if(line.contains("$GPRMC"))
        {
            /*
            osmscout::Routing::Step step = gps_data.getRouteFromBuffer(&outputData);

            PiLibocik::Partition::Node node = NavigationWindow::main->routeWin->routingManager->getPartitionFile()->getNearestNode(PiLibocik::Position(step.lon, step.lat));

            step.id = node.getId();

            //if (node)
            //qDebug() << node.getWaysObj().size();
            //if (!simulationRoute.contains(step))
            if (!simulationRoute.contains(step)) {
                simulationRoute.append(step);
            //    qDebug() << "Node has " << node.getWaysObj().size() << " ways";
            } else {

            }
            */
            osmscout::Routing::Step step = gps_data.getRouteFromBuffer(&outputData);
            step.crossing = false;
            simulationRoute.append(step);

            node = NavigationWindow::main->routeWin->routingManager->getPartitionFile()->getNearestNode(PiLibocik::Position(step.lon, step.lat));
            double actDist = osmscout::Searching::CalculateDistance(node.getLon(), node.getLat(), step.lon, step.lat);
            if (ids.contains(node.getId()) || ids.size() == 0) {
                if (actDist < dist) {
                    dist = actDist;
                    if (lastCross >= 0) {
                        osmscout::Routing::Step s = simulationRoute.at(lastCross);
                        s.crossing = false;
                        simulationRoute.removeAt(lastCross);
                        simulationRoute.insert(lastCross, s);
                        //simulationRoute.at(lastCross).setCrossing(false);
                    }
                    lastCross = i;
                    osmscout::Routing::Step s = simulationRoute.at(i);
                    s.crossing = true;
                    simulationRoute.removeAt(i);
                    simulationRoute.insert(i, s);
                } else {

                }
            }
            else {
                ids.append(node.getId());
                dist = actDist;
                lastCross = i;
                osmscout::Routing::Step s = simulationRoute.at(i);
                s.crossing = true;
                simulationRoute.removeAt(i);
                simulationRoute.insert(i, s);
            }

            i++;
            if (i % 100 == 0) {
                qDebug() << i;
            }
        }
    }

    qDebug() << "SIMULATION ROUTE: " << simulationRoute.length();
    file.close();

    return true;
}

void GPSreceiver::setMode(int m)
{
    disable();
    mode = m;
}

void GPSreceiver::disable()
{
    contiunue = false;
}

