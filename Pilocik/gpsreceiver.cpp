#include "gpsreceiver.h"
#include <windows.h>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QList>
#include <QMetaType>

GPSdata::GPSdata()
{
    lat=lon=alt=speed=angle=0;
    tracedSat=active=0;
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
        active = 1;
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


GPSreceiver::GPSreceiver()
{
}

GPSreceiver::~GPSreceiver()
{
    disable();
    CloseHandle(hInput);
    terminate();
}

void GPSreceiver::setSimPath(QString path)
{
    this->path = path;
}

bool GPSreceiver::startSimulation()
{
    QString lBuffer;

    hInput = CreateFile((LPCTSTR)path.utf16(),                // file to open
               GENERIC_READ,           // open for reading
               FILE_SHARE_READ,        // share for reading
               NULL,                   // default security
               OPEN_EXISTING,          // existing file only
               FILE_ATTRIBUTE_NORMAL,   // overlapped operation
               NULL);                  // no attr. template

    if (hInput == INVALID_HANDLE_VALUE)
    {
        return false;
    }

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
        if(lBuffer.endsWith('\n'))
        {
            if(lBuffer.startsWith("$"))
            {
                output.append(lBuffer);
                if(lBuffer.contains("$GPRMC"))
                {
                    GPSdata gps_data = GPSdata();
                    gps_data.parseBuffer(&output);
                    qRegisterMetaType<GPSdata>("GPSdata");
                    emit positionUpdate(gps_data);
                    Sleep(1000);
                }
                lBuffer.clear();
            }
        }
    }
}

int GPSreceiver::connectSerialPort()
{
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
}

bool GPSreceiver::testSerialPort()
{
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
}

bool GPSreceiver::startRealGPS()
{
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
            if(lBuffer.contains("$GPRMC"))
            {
                emit positionUpdate(GPSdata());
            }
            lBuffer.clear();
        }
    }
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
    }
}

void GPSreceiver::clearBuffer()
{
    output.clear();
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

QStringList* GPSreceiver::getOutputBuffer()
{
    return &output;
}
